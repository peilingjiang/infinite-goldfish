int xwidth = 4;
class Blob {
  float minx;
  float miny;
  float maxx;
  float maxy;

  ArrayList<PVector> points;

  Blob(float x, float y) {
    minx = x;
    miny = y;
    maxx = x;
    maxy = y;
    points = new ArrayList<PVector>();
    points.add(new PVector(x, y));
  }
 
 void show() {
    stroke(255);
    noFill();
    //fill(200, 50, 50, 50);
    strokeWeight(2);
    rectMode(CORNERS);
    rect(minx, miny, maxx, maxy);
    line(((minx + maxx)/2) - xwidth,((miny + maxy)/2) - xwidth,((minx + maxx)/2) + xwidth,((miny + maxy)/2) + xwidth);
    line(((minx + maxx)/2) + xwidth,((miny + maxy)/2) - xwidth,((minx + maxx)/2) - xwidth,((miny + maxy)/2) + xwidth);
    //for (PVector v : points) {
    //  stroke(200, 50, 50);
    //  point(v.x, v.y + 360);
    //}
  }
 
 float returnx(){
   return (minx + maxx)/2;
 }
 
 float returny(){
   return (miny + maxy)/2;
 }

  void add(float x, float y) {
    points.add(new PVector(x, y));
    minx = min(minx, x);
    miny = min(miny, y);
    maxx = max(maxx, x);
    maxy = max(maxy, y);
  }

  float size() {
    return (maxx-minx)*(maxy-miny);
  }

  boolean isNear(float x, float y) {
    float d = 10000000;
    for (PVector v : points) {
      float tempD = distSq(x, y, v.x, v.y);
      if (tempD < d) {
        d = tempD;
      }
    }

    if (d < distThreshold*distThreshold) {
      return true;
    } else {
      return false;
    }
  }
}
