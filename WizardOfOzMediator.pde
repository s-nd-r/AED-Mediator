color fillVal = color(126);
import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[2];  // USB port on the right of Mac
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  fill(fillVal);
  rect(0, 0, 200, 200);
}

void keyPressed() {
  if (key == '1') {
    myPort.write('1');
    fillVal = color(255);
    
  } else if (key == '2') {
    myPort.write('2');
    fillVal = color(0);
    
  } else if (key == '3') {
    myPort.write('3');
    fillVal = color(10);
    
  } else if (key == '4') {
    myPort.write('4');
    fillVal = color(240);
    
  } else if (key == '5') {
    myPort.write('5');
    fillVal = color(30);
    
  } else if (key == '6') {
    myPort.write('6');
    fillVal = color(50);
    
  } else if (key == 'z') {
    myPort.write('d');
    fillVal = color(10);
    
  } else if (key == 'x') {
    myPort.write('a');
    fillVal = color(240);
    
  } else if (key == 'c') {
    myPort.write('p');
    fillVal = color(30);
    
  } else if (key == 'q') {
    myPort.write('r');
    fillVal = color(50);
    
  } else if (key == 'a') {
    myPort.write('n');
    fillVal = color(80);
    
  } else if (key == 'p') {
    myPort.write('k');
    fillVal = color(100);
    
  } else if (key == 'f') {
    myPort.write('f');
    fillVal = color(140);
    
  } else if (key == 'g') {
    myPort.write('g');
    fillVal = color(160);
    
  } else if (key == '8') {
    myPort.write('8');
    fillVal = color(180);
    
  } else if (key == '9') {
    myPort.write('9');
    fillVal = color(200);
    
  } else if (key == '0') {
    myPort.write('0');
    fillVal = color(230);
    
  }
}
