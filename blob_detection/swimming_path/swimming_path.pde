import processing.video.*;

Movie movie;
color trackColor; 
float threshold = 45;
float distThreshold = 50;
float xPadding;
float yPadding;
float px = 0;
float py = 0;
boolean detected = false;
int duration = 30;
int count = 0;
int letterX;
int letterY;

ArrayList<Blob> blobs = new ArrayList<Blob>();
StringList letters = new StringList();//26*26
char[][] letterTable = new char[26][26];

void initialTable(StringList letters) {
  for (char i = 65; i < 91; i ++) {
    for (char j = 0; j < 26; j ++) {
      String s = Character.toString(i);
      letters.append(s);
    }
  }
  for (char x = 0; x < 26; x ++) {
    for (char y = 0; y < 26; y ++) {
      int whichLetter = int(random(letters.size()));
      letterTable[x][y] = letters.get(whichLetter).charAt(0);
      letters.remove(whichLetter);
    }
  }
}


void setup() {
  background(255);
  size(700, 700);
  xPadding = width/26;
  yPadding = height/26;
  movie = new Movie(this, "goldfish.mp4");
  movie.play();
  trackColor = color(10);
  initialTable(letters);
}

void drawTable() {
  for (char x = 0; x < 26; x ++) {
    for (char y = 0; y < 26; y ++) {
      for (char z = 0; z < 26; z ++) {
        textAlign(CENTER);
        fill(180);
        textSize(15);
        text(letterTable[x][y], (0.5 + x)*xPadding, (0.5 + y)*yPadding + 5 );
      }
    }
  }
}

void movieEvent(Movie movie) {
  movie.read();
  movie.loadPixels();
  movie.filter(GRAY);
}

void draw() {
  fill(255,15);
  rect(0,0,width,height);
  
  //tint(255, 50);
  //image(movie, 0, 0,width,height);
  blobs.clear();

  // Begin loop to walk through every pixel
  for (int x = 0; x < movie.width; x++ ) {
    for (int y = 0; y < movie.height; y++ ) {
      int loc = x + y * movie.width;
      // What is current color
      color currentColor = movie.pixels[loc];
      float r1 = red(currentColor);
      float g1 = green(currentColor);
      float b1 = blue(currentColor);
      float r2 = red(trackColor);
      float g2 = green(trackColor);
      float b2 = blue(trackColor);

      float d = distSq(r1, g1, b1, r2, g2, b2); 

      if (d < threshold*threshold) {
        boolean found = false;
        for (Blob b : blobs) {
          if (b.isNear(x, y)) {
            b.add(x, y);
            found = true;
            break;
          }
        }

        if (!found) {
          Blob b = new Blob(x, y);
          blobs.add(b);
        }
      }
    }
  }
   drawTable();

    for (Blob b : blobs) {
      if (b.size() > 8000) {
        if(!detected) {
          px = b.returnx();
          py = b.returny();
          detected = true;
        }
        px = lerp(px, b.returnx(), 0.3);
        py = lerp(py, b.returny(), 0.3);
        noStroke();
        fill(200,50);
        if ((duration - count)<=5) {
          if ( (duration - count)==5) {
            letterX = floor(b.returnx()/xPadding);
            letterY = floor(b.returny()/yPadding);
          }
          if (count == duration) {
            count = 0;
          }
          textAlign(CENTER);
          fill(30);
          textSize(15);
          text(letterTable[letterX][letterY], (0.5 + letterX)*xPadding, (0.5 + letterY)*yPadding + 5 );
          fill(200, 50, 50, 50);
        }
        ellipse(px, py, 20, 20);
        //rectMode(CORNER);
        //rect(x*xPadding, y*yPadding, xPadding, yPadding);
      }
    }
    count ++;
}



// Custom distance functions w/ no square root for optimization
float distSq(float x1, float y1, float x2, float y2) {
  float d = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
  return d;
}


float distSq(float x1, float y1, float z1, float x2, float y2, float z2) {
  float d = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) +(z2-z1)*(z2-z1);
  return d;
}

//void mousePressed() {
//  // Save color where the mouse is clicked in trackColor variable
//  int loc = mouseX + mouseY*movie.width;
//  trackColor = movie.pixels[loc];
//}
