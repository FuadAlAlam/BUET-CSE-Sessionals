#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

class Rectangle{
    double length, width;
    char* color;
    public:
    Rectangle(){
        color = NULL;
    }
    Rectangle(double a, double b, const char* str){
        length=a;
        width=b;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    Rectangle(const Rectangle &r){
        length=r.length;
        width=r.width;
        color = new char[strlen(r.color)+1];
        strcpy(color, r.color);
    }
    ~Rectangle(){
        delete [] color;
    }
    double getPerimeter(){
        return 2*(length+width);
    }
    double getArea(){
        return length*width;
    }
    char* getColor(){
        return color;
    }
    void setColor(const char* str){
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    void set_values(double a, double b, const char* str){
        length=a;
        width=b;
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    double getLength(){
        return length;
    }
    double getWidth(){
        return width;
    }
    Rectangle* clone(){
        Rectangle *r = new Rectangle(*this);
        return r;
    }
};

class Triangle{
    double a, b, c;
    char* color;
    public:
    Triangle(){
        color = NULL;
    }
    Triangle(double p, double q, double r, const char* str){
        a=p;
        b=q;
        c=r;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    Triangle(const Triangle &t){
        a=t.a;
        b=t.b;
        c=t.c;
        color = new char[strlen(t.color)+1];
        strcpy(color, t.color);
    }
    ~Triangle(){
        delete [] color;
    }
    double getPerimeter(){
        return a+b+c;
    }
    double getArea(){
        double s=(a+b+c)/2;
        double area = sqrt(s*(s-a)*(s-b)*(s-c));
        return area;
    }
    char* getColor(){
        return color;
    }
    void setColor(const char* str){
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    void set_values(double p, double q, double r, const char* str){
        a=p; b=q; c=r;
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    double get_a(){
        return a;
    }
    double get_b(){
        return b;
    }
    double get_c(){
        return c;
    }
    Triangle* clone(){
        Triangle *t = new Triangle(*this);
        return t;
    }
};

class Circle{
    double radius;
    char* color;
    public:
    Circle(){
        color = NULL;
    }
    Circle(double r, const char* str){
        radius=r;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    Circle(const Circle &c){
        radius=c.radius;
        color = new char[strlen(c.color)+1];
        strcpy(color, c.color);
    }
    ~Circle(){
        delete [] color;
    }
    double getPerimeter(){
        return 2 * 3.14 * radius;
    }
    double getArea(){
        return radius * radius * 3.14;
    }
    char* getColor(){
        return color;
    }
    void setColor(const char* str){
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    void set_values(double r, const char* str){
        radius=r;
        delete [] color;
        color = new char[strlen(str)+1];
        strcpy(color, str);
    }
    double getRadius(){
        return radius;
    }
    Circle* clone(){
        Circle *c = new Circle(*this);
        return c;
    }
};

class ShapeCollection{
    Rectangle **rectangles;
    Triangle **triangles;
    Circle **circles;
    int RectCount, TriCount, CircCount, RectCapacity, TriCapacity, CircCapacity;
    public:
    ShapeCollection(){
        RectCount=0; TriCount=0; CircCount=0;
        RectCapacity=1; TriCapacity=1; CircCapacity=1;
        rectangles = new Rectangle*[RectCapacity];
        triangles = new Triangle*[TriCapacity];
        circles = new Circle*[CircCapacity];
    }

    void RectResize(){
        RectCapacity *= 2;
        Rectangle **temp = new Rectangle*[RectCapacity];
        for(int i=0; i<RectCount; i++){
            temp[i] = rectangles[i]->clone();
        }
        delete [] rectangles;
        rectangles = temp;

        cout<<"Increasing capacity of rectangles from "<<RectCount<<" to "<<RectCapacity<<endl;
    }
    void add(Rectangle r){
        if(RectCount == RectCapacity) RectResize();

        rectangles[RectCount] = r.clone();
        RectCount++;
    }

    void TriResize(){
        TriCapacity *= 2;
        Triangle **temp = new Triangle*[TriCapacity];
        for(int i=0; i<TriCount; i++){
            temp[i] = triangles[i]->clone();
        }
        delete [] triangles;
        triangles = temp;

        cout<<"Increasing capacity of triangles from "<<TriCount<<" to "<<TriCapacity<<endl;
    }
    void add(Triangle t){
        if(TriCount == TriCapacity) TriResize();

        triangles[TriCount] = t.clone();
        TriCount++;
    }

    void CircResize(){
        CircCapacity *= 2;
        Circle **temp = new Circle*[CircCapacity];
        for(int i=0; i<CircCount; i++){
            temp[i] = circles[i]->clone();
        }
        delete [] circles;
        circles = temp;

        cout<<"Increasing capacity of circles from "<<CircCount<<" to "<<CircCapacity<<endl;
    }
    void add(Circle c){
        if(CircCount == CircCapacity) CircResize();

        circles[CircCount] = c.clone();
        CircCount++;
    }

    int getRectCount(){
        return RectCount;
    }
    int getTriCount(){
        return TriCount;
    }
    int getCircCount(){
        return CircCount;
    }
    void printRectangles(){
        for(int i=0; i<RectCount; i++){
            cout<<"Rectangle "<<i<<": length: "<<rectangles[i]->getLength()<<" width: "<<rectangles[i]->getWidth()<<endl;
        }
    }
    void printTriangles(){
        for(int i=0; i<TriCount; i++){
            cout<<"Triangle "<<i<<": a: "<<triangles[i]->get_a()<<" b: "<<triangles[i]->get_b()<<" c: "<<triangles[i]->get_c()<<endl;
        }
    }
    void printCircles(){
        for(int i=0; i<CircCount; i++){
            cout<<"Circle "<<i<<": radius: "<<circles[i]->getRadius()<<endl;
        }
    }
    ~ShapeCollection(){
        for(int i=0; i<RectCount; i++){
            delete rectangles[i];
        }
        delete [] rectangles;

        for(int i=0; i<TriCount; i++){
            delete triangles[i];
        }
        delete [] triangles;

        for(int i=0; i<CircCount; i++){
            delete circles[i];
        }
        delete [] circles;
    }
};

int main() {
    // Create rectangle with length, width, color
    Rectangle r1(10, 20, "Red");
    // The Color is stored using malloc, which will be freed during destruction
    cout << "Rectangle Perimeter: " << r1.getPerimeter() << endl;
    cout << "Rectangle Area: " << r1.getArea() << endl;
    cout << "Rectangle Color: " << r1.getColor() << endl;
    // When changing the color, you need to free the memory of the old color
    // and allocate new memory for the new color
    r1.setColor("Yellow");
    cout << "Rectangle Color: " << r1.getColor() << endl;
    cout << "--------------------------------------" << endl;
 
    // Create triangle with a, b, c, color. (a, b, c are lengths of the sides)
    Triangle t1(3, 4, 5, "Blue");
    cout << "Triangle Perimeter: " << t1.getPerimeter() << endl;
    cout << "Triangle Color: " << t1.getColor() << endl;
    cout << "Triangle Area: " << t1.getArea() << endl;
    t1.setColor("Orange");
    cout << "Triangle Color: " << t1.getColor() << endl;
    cout << "--------------------------------------" << endl;
 
    // Create circle with radius, color
    Circle c1(7, "Green");
    cout << "Circle Perimeter: " << c1.getPerimeter() << endl;
    cout << "Circle Area: " << c1.getArea() << endl;
    cout << "Circle Color: " << c1.getColor() << endl;
    c1.setColor("Purple");
    cout << "Circle Color: " << c1.getColor() << endl;
    cout << "--------------------------------------" << endl;
 
    /*
    When constructing the ShapeCollection class, you will create dynamic for 
    rectangles, triangles and circles. You have to dynamically allocate memory for this.
    */ 
    ShapeCollection shapes;
    /* IMPORTANT: You need to pass the objects by value to the add functions
    If you pass by value, the copy constructor will be called and the dynamically
    allocated memory will be copied. So, you have to write copy contructor so that 
    memory allocation is properly done and no double free error occurs.
    */ 
 
 
    shapes.add(r1);
    shapes.add(t1);
    shapes.add(c1);
 
    Rectangle r2(15, 25, "Black");
    Rectangle r3(150, 225, "Green");
    shapes.add(r2);
    shapes.add(r3);
    Triangle t2(5, 12, 13, "White");
    shapes.add(t2);
 
    cout << "Number of Rectangles: " << shapes.getRectCount() << endl;
    cout << "Number of Triangles: " << shapes.getTriCount() << endl;
    cout << "Number of Circles: " << shapes.getCircCount() << endl;
    cout << "--------------------------------------" << endl;
 
    shapes.printRectangles();
    shapes.printTriangles();
    shapes.printCircles();
 
    return 0;
}