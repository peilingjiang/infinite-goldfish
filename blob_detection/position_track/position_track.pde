import processing.video.*;

Movie movie;
color trackColor; 
float threshold = 20;
float distThreshold = 50;

ArrayList<Blob> blobs = new ArrayList<Blob>();

void setup() {
  background(255);
  size(640, 360);
  movie = new Movie(this, "goldfish.mov");
  movie.play();
  trackColor = color(255, 0, 0);
}

void movieEvent(Movie movie){
  movie.read();
  movie.loadPixels();
}

void draw() {
  fill(255,50);
  movie.filter(GRAY);
  image(movie, 0, 0,width,height);
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

  for (Blob b : blobs) {
    if (b.size() > 500) {
      b.show();
    }
  }
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

void mousePressed() {
  // Save color where the mouse is clicked in trackColor variable
  int loc = mouseX + mouseY*movie.width;
  trackColor = movie.pixels[loc];
}
