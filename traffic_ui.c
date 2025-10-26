#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> 

#include "raylib-5.5_win64_mingw-w64/include/raylib.h" 

struct Signal {
    char direction[50];
    int greenDuration;     
    int yellowDuration;
    int vehicleCount;      
};
struct Signal createSignal(const char* dir, int green, int yellow, int vehicles) {
    struct Signal s;
    strcpy(s.direction, dir);
    s.greenDuration = green;
    s.yellowDuration = yellow;
    s.vehicleCount = vehicles;
    return s;
}
struct Signal createEmptySignal() { return createSignal("", 0, 0, 0); }

struct CircularQueue {
    struct Signal* queueArray;
    int maxSize;
    int front;
    int rear;
    int itemCount;
};
void initQueue(struct CircularQueue* q, int size) {
    q->maxSize = size;
    q->queueArray = (struct Signal*)malloc(size * sizeof(struct Signal));
    q->front = 0;
    q->rear = -1;
    q->itemCount = 0;
}
void freeQueue(struct CircularQueue* q) { free(q->queueArray); }
bool isFull(struct CircularQueue* q) { return (q->itemCount == q->maxSize); }
bool isEmpty(struct CircularQueue* q) { return (q->itemCount == 0); }
void enqueue(struct CircularQueue* q, struct Signal signal) {
    if (isFull(q)) return;
    if (q->rear == q->maxSize - 1) q->rear = -1;
    q->queueArray[++q->rear] = signal;
    q->itemCount++;
}
struct Signal dequeue(struct CircularQueue* q) {
    if (isEmpty(q)) return createEmptySignal();
    struct Signal frontItem = q->queueArray[q->front]; 
    if (q->front == q->maxSize - 1) q->front = 0;
    else q->front++;
    q->itemCount--;
    return frontItem;
}

#define STATE_GREEN  0
#define STATE_YELLOW 1
#define MAX_VEHICLES 20 

void DrawTrafficLight(Vector2 pos, bool isVertical, const char* direction, const char* activeDir, int activeState) {
    bool isActive = (strcmp(direction, activeDir) == 0);
    Color redColor    = !isActive ? RED : Fade(RED, 0.3f);
    Color yellowColor = isActive && activeState == STATE_YELLOW ? YELLOW : Fade(YELLOW, 0.3f);
    Color greenColor  = isActive && activeState == STATE_GREEN ? GREEN : Fade(GREEN, 0.3f);

    if (isVertical) {
        DrawRectangle(pos.x - 20, pos.y - 60, 40, 120, BLACK);
        DrawCircle(pos.x, pos.y - 40, 15, redColor);
        DrawCircle(pos.x, pos.y, 15, yellowColor);
        DrawCircle(pos.x, pos.y + 40, 15, greenColor);
    } else {
        DrawRectangle(pos.x - 60, pos.y - 20, 120, 40, BLACK);
        DrawCircle(pos.x - 40, pos.y, 15, redColor);
        DrawCircle(pos.x, pos.y, 15, yellowColor);
        DrawCircle(pos.x + 40, pos.y, 15, greenColor);
    }
}

void DrawVehicles(const char* direction, int totalCount, int departedCount, Rectangle road, Vector2 carSizeBase) {
    if (totalCount > MAX_VEHICLES) totalCount = MAX_VEHICLES; 

    Vector2 carSize = carSizeBase; 
    
    if (strcmp(direction, "North") == 0 || strcmp(direction, "South") == 0) {
        float temp = carSize.x;
        carSize.x = carSize.y; 
        carSize.y = temp;      
    }

    for (int i = departedCount; i < totalCount; i++) {
        Vector2 pos = {0, 0};
        
        Color carColor = (Color){ 
            (unsigned char)(100 + (i * 30) % 155),
            (unsigned char)(120 + (i * 40) % 135),
            (unsigned char)(150 + (i * 25) % 105),
            255 
        };
        
        int queuePosition = i - departedCount;

        if (strcmp(direction, "North") == 0) {
            pos.x = road.x + 15;
            pos.y = road.y + road.height - 40 - (queuePosition * (carSize.y + 5));
        } else if (strcmp(direction, "South") == 0) {
            pos.x = road.x + 65;
            pos.y = road.y + 15 + (queuePosition * (carSize.y + 5));
        } else if (strcmp(direction, "West") == 0) {
            pos.x = road.x + road.width - 40 - (queuePosition * (carSize.x + 5));
            pos.y = road.y + 55;
        } else if (strcmp(direction, "East") == 0) {
            pos.x = road.x + 15 + (queuePosition * (carSize.x + 5));
            pos.y = road.y + 10;
        }
        DrawRectangleV(pos, carSize, carColor);
        DrawRectangleLines((int)pos.x, (int)pos.y, (int)carSize.x, (int)carSize.y, BLACK);
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    srand(time(NULL)); 
    InitWindow(screenWidth, screenHeight, "Adaptive Traffic Light UI Simulation");
    
    struct CircularQueue trafficSystem;
    initQueue(&trafficSystem, 4);

    enqueue(&trafficSystem, createSignal("North", 4, 2, 10));
    enqueue(&trafficSystem, createSignal("East", 3, 2, 8));
    enqueue(&trafficSystem, createSignal("South", 4, 2, 12));
    enqueue(&trafficSystem, createSignal("West", 3, 2, 5));

    Rectangle roadNorth = { 350, 0, 100, 250 };
    Rectangle roadSouth = { 350, 350, 100, 250 };
    Rectangle roadWest = { 0, 250, 350, 100 };
    Rectangle roadEast = { 450, 250, 350, 100 };
    Rectangle intersection = { 350, 250, 100, 100 };
    Rectangle stopNorth = { 350, 240, 50, 10 };
    Rectangle stopSouth = { 400, 350, 50, 10 };
    Rectangle stopWest = { 340, 300, 10, 50 };
    Rectangle stopEast = { 450, 250, 10, 50 };
    Vector2 lightPosNorth = { 470, 230 };
    Vector2 lightPosSouth = { 330, 370 };
    Vector2 lightPosWest =  { 330, 230 };
    Vector2 lightPosEast =  { 470, 370 };
    Vector2 carSize = { 35, 20 }; 

    int currentState = STATE_GREEN;      
    float timer = 0.0f;
    float actualGreenDuration = 0.0f;
    float carReleaseTimer = 0.0f;
    float newCarTimer = 0.0f;     
    int carsDeparted = 0; 
    struct Signal currentSignal = dequeue(&trafficSystem);
    SetTargetFPS(60); 

    while (!WindowShouldClose()) { 
        
        float frameTime = GetFrameTime();
        timer += frameTime; 
        carReleaseTimer += frameTime;
        newCarTimer += frameTime;

        if (newCarTimer > 1.5f) {
            struct CircularQueue tempQ = trafficSystem; 
            for(int i = 0; i < tempQ.itemCount; i++) {
                int queueIndex = (trafficSystem.front + i) % trafficSystem.maxSize;
                
                trafficSystem.queueArray[queueIndex].vehicleCount += GetRandomValue(0, 2); 
                if (trafficSystem.queueArray[queueIndex].vehicleCount > MAX_VEHICLES) {
                    trafficSystem.queueArray[queueIndex].vehicleCount = MAX_VEHICLES;
                }
            }
            newCarTimer = 0.0f;
        }

        if (currentState == STATE_GREEN) {
            if (timer < 0.1f) { 
                actualGreenDuration = (float)currentSignal.greenDuration + ((float)currentSignal.vehicleCount * 0.5f);
                carsDeparted = 0; 
            }
            
            if (carReleaseTimer > 0.5f && carsDeparted < currentSignal.vehicleCount) {
                carsDeparted++;
                carReleaseTimer = 0.0f;
            }

            if (timer >= actualGreenDuration || (carsDeparted == currentSignal.vehicleCount && timer > currentSignal.greenDuration)) {
                currentState = STATE_YELLOW;
                timer = 0.0f;
            }
        } 
        else if (currentState == STATE_YELLOW) {
            if (timer >= currentSignal.yellowDuration) {
                
                currentSignal.vehicleCount -= carsDeparted;
                if (currentSignal.vehicleCount < 0) currentSignal.vehicleCount = 0;

                int newArrivals = rand() % 5; 
                currentSignal.vehicleCount += newArrivals;
                if (currentSignal.vehicleCount > MAX_VEHICLES) {
                    currentSignal.vehicleCount = MAX_VEHICLES;
                }

                enqueue(&trafficSystem, currentSignal); 
                currentSignal = dequeue(&trafficSystem);
                currentState = STATE_GREEN;
                timer = 0.0f; 
                carReleaseTimer = 0.0f;
                carsDeparted = 0; 
            }
        }
        
        float timeLeft = 0.0f;
        if (currentState == STATE_GREEN) timeLeft = actualGreenDuration - timer;
        else timeLeft = currentSignal.yellowDuration - timer;
        if (timeLeft < 0.0f) timeLeft = 0.0f; 

        BeginDrawing();
            ClearBackground(DARKGREEN); 
            
            DrawRectangleRec(roadNorth, GRAY);
            DrawRectangleRec(roadSouth, GRAY);
            DrawRectangleRec(roadWest, GRAY);
            DrawRectangleRec(roadEast, GRAY);
            DrawRectangleRec(intersection, GRAY); 
            DrawRectangle(395, 0, 10, 240, Fade(WHITE, 0.3f));
            DrawRectangle(395, 360, 10, 240, Fade(WHITE, 0.3f));
            DrawRectangle(0, 295, 340, 10, Fade(WHITE, 0.3f));
            DrawRectangle(460, 295, 340, 10, Fade(WHITE, 0.3f));
            DrawRectangleRec(stopNorth, WHITE);
            DrawRectangleRec(stopSouth, WHITE);
            DrawRectangleRec(stopWest, WHITE);
            DrawRectangleRec(stopEast, WHITE);
            
            DrawText("NORTH", 355, 10, 30, Fade(WHITE, 0.7f));
            DrawText("SOUTH", 355, 560, 30, Fade(WHITE, 0.7f));
            DrawText("WEST", 10, 360, 30, Fade(WHITE, 0.7f));
            DrawText("EAST", 680, 210, 30, Fade(WHITE, 0.7f));
            
            DrawTrafficLight(lightPosNorth, false, "North", currentSignal.direction, currentState);
            DrawTrafficLight(lightPosSouth, false, "South", currentSignal.direction, currentState);
            DrawTrafficLight(lightPosWest, true, "West", currentSignal.direction, currentState);
            DrawTrafficLight(lightPosEast, true, "East", currentSignal.direction, currentState);

            int carsToDraw = currentSignal.vehicleCount;
            if (carsToDraw > MAX_VEHICLES) carsToDraw = MAX_VEHICLES;
            
            if(strcmp(currentSignal.direction, "North") == 0) DrawVehicles("North", carsToDraw, carsDeparted, roadNorth, carSize);
            if(strcmp(currentSignal.direction, "South") == 0) DrawVehicles("South", carsToDraw, carsDeparted, roadSouth, carSize);
            if(strcmp(currentSignal.direction, "West") == 0) DrawVehicles("West", carsToDraw, carsDeparted, roadWest, carSize);
            if(strcmp(currentSignal.direction, "East") == 0) DrawVehicles("East", carsToDraw, carsDeparted, roadEast, carSize);

            struct CircularQueue tempQ = trafficSystem; 
            for(int i = 0; i < tempQ.itemCount; i++) {
                struct Signal s = dequeue(&tempQ);
                carsToDraw = s.vehicleCount;
                if (carsToDraw > MAX_VEHICLES) carsToDraw = MAX_VEHICLES;

                if (strcmp(s.direction, "North") == 0) DrawVehicles("North", carsToDraw, 0, roadNorth, carSize);
                if (strcmp(s.direction, "South") == 0) DrawVehicles("South", carsToDraw, 0, roadSouth, carSize);
                if (strcmp(s.direction, "West") == 0) DrawVehicles("West", carsToDraw, 0, roadWest, carSize);
                if (strcmp(s.direction, "East") == 0) DrawVehicles("East", carsToDraw, 0, roadEast, carSize);
            }

            char movementText[100] = "";
            if (currentState == STATE_GREEN) {
                if (strcmp(currentSignal.direction, "North") == 0) strcpy(movementText, "Flow: N -> S (Straight)\n          N -> W (Right)"); 
                else if (strcmp(currentSignal.direction, "East") == 0) strcpy(movementText, "Flow: E -> W (Straight)\n          E -> N (Right)"); 
                else if (strcmp(currentSignal.direction, "South") == 0) strcpy(movementText, "Flow: S -> N (Straight)\n          S -> E (Right)"); 
                else if (strcmp(currentSignal.direction, "West") == 0) strcpy(movementText, "Flow: W -> E (Straight)\n          W -> S (Right)"); 
            } else {
                strcpy(movementText, "Flow: (Clearing)");
            }

            DrawRectangle(5, 5, 250, 205, Fade(BLACK, 0.7f)); 
            
            int remainingCars = currentSignal.vehicleCount - carsDeparted;
            if (remainingCars < 0) remainingCars = 0;
            if (remainingCars > MAX_VEHICLES) remainingCars = MAX_VEHICLES;
            
            DrawText(TextFormat("Current: %s", currentSignal.direction), 15, 15, 20, WHITE);
            DrawText(TextFormat("Vehicles: %d", remainingCars), 15, 45, 20, LIME);
            DrawText(TextFormat("State: %s", currentState == STATE_GREEN ? "GREEN" : "YELLOW"), 15, 75, 20, WHITE);
            DrawText(TextFormat("Duration: %.1f s", (currentState == STATE_GREEN) ? actualGreenDuration : currentSignal.yellowDuration), 15, 105, 20, LIGHTGRAY);
            DrawText(TextFormat("Time Left: %.1f s", timeLeft), 15, 135, 20, WHITE);
            DrawText(movementText, 15, 165, 20, SKYBLUE);

        EndDrawing();
    }

    CloseWindow();        
    freeQueue(&trafficSystem); 

    return 0;
}