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
        color = new char[strlen(str)];
        strcpy(color, str);
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
        color = new char[strlen(str)];
        strcpy(color, str);
    }
    void set_values(double a, double b, const char* str){
        length=a;
        width=b;
        color = new char[strlen(str)];
        strcpy(color, str);
    }
    double getLength(){
        return length;
    }
    double getWidth(){
        return width;
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
        color = new char[strlen(str)];
        strcpy(color, str);
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
        color = new char[strlen(str)];
        strcpy(color, str);
    }
    void set_values(double p, double q, double r, const char* str){
        a=p; b=q; c=r;
        color = new char[strlen(str)];
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
        color = new char[strlen(str)];
        strcpy(color, str);
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
        color = new char[strlen(str)];
        strcpy(color, str);
    }
    void set_values(double r, const char* str){
        radius=r;
        color = new char[strlen(str)];
        strcpy(color, str);
    }
    double getRadius(){
        return radius;
    }
};

class ShapeCollection{
    Rectangle rectangles[100];
    Triangle triangles[100];
    Circle circles[100];
    int RectCount, TriCount, CircCount;
    public:
    ShapeCollection(){
        RectCount=0; TriCount=0; CircCount=0;
    }
    void addRectangle(Rectangle &r){
        rectangles[RectCount].set_values(r.getLength(), r.getWidth(), r.getColor());
        RectCount++;
    }
    void addTriangle(Triangle &t){
        triangles[TriCount].set_values(t.get_a(), t.get_b(), t.get_c(), t.getColor());
        TriCount++;
    }
    void addCircle(Circle &c){
        circles[CircCount].set_values(c.getRadius(), c.getColor());
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
            cout<<"Rectangle "<<i<<": length: "<<rectangles[i].getLength()<<" width: "<<rectangles[i].getWidth()<<endl;
        }
    }
    void printTriangles(){
        for(int i=0; i<TriCount; i++){
            cout<<"Triangle "<<i<<": a: "<<triangles[i].get_a()<<" b: "<<triangles[i].get_b()<<" c: "<<triangles[i].get_c()<<endl;
        }
    }
    void printCircles(){
        for(int i=0; i<CircCount; i++){
            cout<<"Circle "<<i<<": radius: "<<circles[i].getRadius()<<endl;
        }
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
    When constructing the ShapeCollection class, you will create static arrays for 100 
    rectangles, triangles and circles. You don’t have to dynamically allocate memory for this.
    */ 
    ShapeCollection shapes;
    /* IMPORTANT: You need to pass the objects by reference to the add functions
    If you pass by value, the copy constructor will be called and the dynamically
    allocated memory will be copied, leading to double free errors when things go 
    out of scope. Once passed by reference, do not directly store the reference in 
    the array. Instead, copy the data from the reference to the array element.
    We will see better ways to handle this in the upcoming sessions.
    */ 
    shapes.addRectangle(r1);
    shapes.addTriangle(t1);
    shapes.addCircle(c1);

    Rectangle r2(15, 25, "Black");
    shapes.addRectangle(r2);
    Triangle t2(5, 12, 13, "White");
    shapes.addTriangle(t2);

    cout << "Number of Rectangles: " << shapes.getRectCount() << endl;
    cout << "Number of Triangles: " << shapes.getTriCount() << endl;
    cout << "Number of Circles: " << shapes.getCircCount() << endl;
    cout << "--------------------------------------" << endl;

    shapes.printRectangles();
    shapes.printTriangles();
    shapes.printCircles();

    return 0;
}