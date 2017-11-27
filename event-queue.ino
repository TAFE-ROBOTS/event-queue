//Requires LinkedList library
#include <LinkedList.h>

#define LEDPIN_RED      5
#define LEDPIN_YELLOW   6
#define LEDPIN_GREEN    7

//Define the class to place on the queue
class Event {
  public:
    long timestamp;
    int param;
    void(*func)(int);
};

//Define the queue
LinkedList<Event*> eventQueue = LinkedList<Event*>();

//Define the shape of the add function
void addEvent(void (*)(int), int param, long wait);

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN_RED, OUTPUT);
  pinMode(LEDPIN_YELLOW, OUTPUT);
  pinMode(LEDPIN_GREEN, OUTPUT);
  //Kick off your scheduled behaviours
  start();
}

//Sorting function
int timeIndexed(Event *&eventA, Event *&eventB) {
  return eventA->timestamp - eventB->timestamp;
}

//Add a single event
void addEvent(void (*function)(int), int param, long wait) {
  Event *event = new Event();
  event->timestamp = wait+millis();
  event->func = function;
  event->param = param;
  eventQueue.add(event);
  eventQueue.sort(timeIndexed);
}

void loop() {

  //Read the top event and fire it
  Event *topEvent = eventQueue.get(0);
  if (topEvent->timestamp < millis()) {
    Event *eventToPlay = eventQueue.shift();
    eventToPlay->func(eventToPlay->param);
    delete eventToPlay;
  } else {
    Serial.println("waiting");
  }

}

// YOUR ROBOT CODE BELOW THIS LINE
//---------------------------------
void start() {
  flashRed(1);
  flashYellow(0);
  flashGreen(1);
}

void flashRed(int val) {
  digitalWrite(LEDPIN_RED, !digitalRead(LEDPIN_RED));   // toggle pin state
  val=val+50;
  if (val > 500) {
    val = 10;
  }
  addEvent(flashRed, val, val);
}

void flashYellow(int state){
  digitalWrite(LEDPIN_YELLOW, state);   // set pin state
  addEvent(flashYellow, !state, 100);
}

void flashGreen(int state){
  digitalWrite(LEDPIN_GREEN, state);   // set pin state
  addEvent(flashGreen, !state, 150);
}
