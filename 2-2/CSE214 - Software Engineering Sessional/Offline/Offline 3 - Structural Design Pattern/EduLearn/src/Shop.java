import java.util.*;

import Education.EduComponent;
import Education.Lesson;
import Education.Course;
import Education.Module;
import AddOns.*;
import Discounts.*;
import Cart.*;

public class Shop {

    private static boolean isModuleAvailable(int mIdx, Set<Integer> claimedModules,
                                              Set<Integer> claimedCourses, int[][] moduleCourseIdx) {
        if(claimedModules.contains(mIdx)) return false;
        for(int ci : moduleCourseIdx[mIdx]) {
            if(claimedCourses.contains(ci)) return false;
        }
        return true;
    }

    private static void showCartDetails(ConcreteCart cart, List<String> itemDescs) {
        System.out.println("\n  ============= YOUR CART =============");
        if(cart.getItems().isEmpty()) {
            System.out.println("    (empty)");
            System.out.println("  =====================================");
            return;
        }

        for(int i=0; i < itemDescs.size(); i++) {
            System.out.println("    " + (i+1) + ". " + itemDescs.get(i));
            if(i < itemDescs.size()-1) System.out.println();
        }

        System.out.printf("\n    Cart Subtotal: $%.1f | Duration: %.2f hrs%n",
                cart.getTotalPrice(), cart.getTotalDuration());
        System.out.println("  =====================================");
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);


        Course cProg1 = new Course("Introduction to Programming");
        cProg1.addComponent(new Lesson("Variables, Data Types & Operators", 0.2, 20.0));
        cProg1.addComponent(new Lesson("Control Flow: Conditionals & Loops", 0.25, 25.0));
        cProg1.addComponent(new Lesson("Functions & Recursion", 0.3, 25.0));
        cProg1.addComponent(new Lesson("Arrays & Strings", 0.2, 20.0));

        Course cOOP = new Course("Object-Oriented Programming");
        cOOP.addComponent(new Lesson("Classes, Objects & Constructors", 0.3, 30.0));
        cOOP.addComponent(new Lesson("Inheritance & Polymorphism", 0.25, 35.0));
        cOOP.addComponent(new Lesson("Abstraction & Encapsulation", 0.2, 30.0));
        cOOP.addComponent(new Lesson("Interfaces & Generics", 0.2, 25.0));

        Module m1 = new Module("Programming Fundamentals");
        m1.addComponent(cProg1);
        m1.addComponent(cOOP);


        Course cDS = new Course("Data Structures");
        cDS.addComponent(new Lesson("Linked Lists & Stacks", 0.25, 30.0));
        cDS.addComponent(new Lesson("Queues & Priority Queues", 0.2, 25.0));
        cDS.addComponent(new Lesson("Trees & Binary Search Trees", 0.3, 35.0));
        cDS.addComponent(new Lesson("Graphs & Hash Tables", 0.25, 35.0));

        Course cAlgo = new Course("Algorithms");
        cAlgo.addComponent(new Lesson("Sorting & Searching", 0.25, 30.0));
        cAlgo.addComponent(new Lesson("Divide & Conquer and Greedy", 0.2, 35.0));
        cAlgo.addComponent(new Lesson("Dynamic Programming", 0.35, 40.0));
        cAlgo.addComponent(new Lesson("Graph Algorithms (BFS, DFS, Dijkstra)", 0.25, 35.0));

        Module m2 = new Module("Data Structures & Algorithms");
        m2.addComponent(cDS);
        m2.addComponent(cAlgo);


        Course cArch = new Course("Computer Architecture");
        cArch.addComponent(new Lesson("Number Systems & Boolean Algebra", 0.2, 20.0));
        cArch.addComponent(new Lesson("CPU Design & Instruction Sets", 0.3, 30.0));
        cArch.addComponent(new Lesson("Memory Hierarchy & Caching", 0.25, 30.0));

        Course cOS = new Course("Operating Systems");
        cOS.addComponent(new Lesson("Processes & Threads", 0.3, 30.0));
        cOS.addComponent(new Lesson("CPU Scheduling", 0.2, 25.0));
        cOS.addComponent(new Lesson("Memory Management & Virtual Memory", 0.3, 35.0));
        cOS.addComponent(new Lesson("File Systems & I/O", 0.2, 25.0));

        Module m3 = new Module("Computer Architecture & Operating Systems");
        m3.addComponent(cArch);
        m3.addComponent(cOS);


        Course cDBFund = new Course("Database Fundamentals");
        cDBFund.addComponent(new Lesson("ER Modeling & Relational Model", 0.3, 30.0));
        cDBFund.addComponent(new Lesson("SQL: Queries, Joins & Subqueries", 0.4, 35.0));
        cDBFund.addComponent(new Lesson("Normalization & Schema Design", 0.2, 30.0));

        Course cDBAdv = new Course("Advanced Database Concepts");
        cDBAdv.addComponent(new Lesson("Indexing & Query Optimization", 0.25, 30.0));
        cDBAdv.addComponent(new Lesson("Transactions & Concurrency Control", 0.3, 30.0));
        cDBAdv.addComponent(new Lesson("NoSQL & Distributed Databases", 0.2, 25.0));

        Module m4 = new Module("Database Systems");
        m4.addComponent(cDBFund);
        m4.addComponent(cDBAdv);


        Course cNet = new Course("Computer Networks");
        cNet.addComponent(new Lesson("OSI & TCP/IP Models", 0.2, 20.0));
        cNet.addComponent(new Lesson("IP Addressing, Routing & Subnetting", 0.3, 30.0));
        cNet.addComponent(new Lesson("Transport Layer: TCP & UDP", 0.25, 30.0));
        cNet.addComponent(new Lesson("Application Protocols: HTTP, DNS, SMTP", 0.2, 25.0));

        Course cSec = new Course("Cybersecurity Essentials");
        cSec.addComponent(new Lesson("Cryptography: Symmetric & Asymmetric", 0.3, 30.0));
        cSec.addComponent(new Lesson("Network Security & Firewalls", 0.2, 25.0));
        cSec.addComponent(new Lesson("Web Security: XSS, SQL Injection, CSRF", 0.3, 30.0));

        Module m5 = new Module("Networking & Cybersecurity");
        m5.addComponent(cNet);
        m5.addComponent(cSec);


        Course cSE = new Course("Software Engineering Principles");
        cSE.addComponent(new Lesson("SDLC Models: Waterfall, Agile, Scrum", 0.3, 25.0));
        cSE.addComponent(new Lesson("Requirements Engineering & UML", 0.2, 30.0));
        cSE.addComponent(new Lesson("Design Patterns (Creational, Structural, Behavioral)", 0.4, 40.0));
        cSE.addComponent(new Lesson("Testing: Unit, Integration & TDD", 0.3, 30.0));

        Course cDevOps = new Course("DevOps & Version Control");
        cDevOps.addComponent(new Lesson("Git, GitHub & Branching Strategies", 0.2, 20.0));
        cDevOps.addComponent(new Lesson("CI/CD Pipelines", 0.3, 30.0));
        cDevOps.addComponent(new Lesson("Docker & Containerization", 0.25, 30.0));

        Module m6 = new Module("Software Engineering & DevOps");
        m6.addComponent(cSE);
        m6.addComponent(cDevOps);


        Course cFrontend = new Course("Frontend Development");
        cFrontend.addComponent(new Lesson("HTML5 & CSS3 Fundamentals", 0.2, 20.0));
        cFrontend.addComponent(new Lesson("JavaScript & DOM Manipulation", 0.3, 30.0));
        cFrontend.addComponent(new Lesson("React.js: Components & State", 0.4, 35.0));

        Course cBackend = new Course("Backend Development");
        cBackend.addComponent(new Lesson("Node.js & Express.js", 0.3, 30.0));
        cBackend.addComponent(new Lesson("REST API Design & Authentication", 0.25, 30.0));
        cBackend.addComponent(new Lesson("Spring Boot & MVC Architecture", 0.4, 35.0));

        Module m7 = new Module("Web Development (Full Stack)");
        m7.addComponent(cFrontend);
        m7.addComponent(cBackend);


        Course cAIFund = new Course("Artificial Intelligence Foundations");
        cAIFund.addComponent(new Lesson("Search Algorithms: BFS, DFS, A*", 0.3, 30.0));
        cAIFund.addComponent(new Lesson("Knowledge Representation & Logic", 0.25, 30.0));
        cAIFund.addComponent(new Lesson("Probabilistic Reasoning & Bayes Nets", 0.3, 30.0));

        Course cML = new Course("Machine Learning");
        cML.addComponent(new Lesson("Linear Regression & Classification", 0.3, 30.0));
        cML.addComponent(new Lesson("Neural Networks & Deep Learning", 0.4, 45.0));
        cML.addComponent(new Lesson("CNNs, RNNs & Transformers", 0.35, 40.0));
        cML.addComponent(new Lesson("Model Evaluation & Hyperparameter Tuning", 0.2, 25.0));

        Module m8 = new Module("Artificial Intelligence & Machine Learning");
        m8.addComponent(cAIFund);
        m8.addComponent(cML);


        Module[] modules = { m1, m2, m3, m4, m5, m6, m7, m8 };
        Course[] courses = { cProg1, cOOP, cDS, cAlgo, cArch, cOS, cDBFund, cDBAdv,
                             cNet, cSec, cSE, cDevOps, cFrontend, cBackend, cAIFund, cML };


        int[][] moduleCourseIdx = {
            {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}
        };


        int[] courseToModule = new int[courses.length];
        for(int mi=0; mi < modules.length; mi++) {
            for(int ci : moduleCourseIdx[mi]) {
                courseToModule[ci] = mi;
            }
        }



        System.out.println("==============================================");
        System.out.println("       Welcome to EduLearn Platform");
        System.out.println("==============================================\n");
        System.out.println("Available Modules and Courses:\n");
        for(int i=0; i < modules.length; i++) {
            System.out.println("  Module " + (i+1) + ":");
            modules[i].display("    ");
            System.out.println();
        }



        ConcreteCart cart = new ConcreteCart();
        Set<Integer> claimedModules = new HashSet<>();
        Set<Integer> claimedCourses = new HashSet<>();
        List<String> itemDescs = new ArrayList<>();

        while(true) {
            System.out.println("\n----------------------------------------------");
            System.out.println("  1. Add to Cart");
            System.out.println("  2. Checkout");
            System.out.println("  3. Exit");
            System.out.print("Enter choice: ");
            String mainChoice = scanner.nextLine().trim();

            if(mainChoice.equals("3")) {
                System.out.println("\nThank you for visiting EduLearn! Goodbye.");
                break;
            }

            if(mainChoice.equals("1")) {
                System.out.println("\nWhat would you like to add?");
                System.out.println("  1. A Module");
                System.out.println("  2. An Individual Course");
                System.out.print("Enter choice: ");
                String addType = scanner.nextLine().trim();

                if(addType.equals("1")) {
                    System.out.println("\nAvailable Modules:");
                    boolean anyAvailable = false;
                    for(int i=0; i < modules.length; i++) {
                        if(claimedModules.contains(i)) {
                            System.out.printf("  %d. %s  [IN CART]%n",
                                    i+1, modules[i].getTitle());
                        }
                        else {
                            List<String> conflicts = new ArrayList<>();
                            for(int ci : moduleCourseIdx[i]) {
                                if(claimedCourses.contains(ci)) {
                                    conflicts.add(courses[ci].getTitle());
                                }
                            }
                            if(!conflicts.isEmpty()) {
                                System.out.printf("  %d. %s  [CONFLICT: %s already in cart]%n",
                                        i+1, modules[i].getTitle(),
                                        String.join(", ", conflicts));
                            }
                            else {
                                anyAvailable = true;
                                System.out.printf("  %d. %s (%.2f hrs, $%.1f)%n",
                                        i+1, modules[i].getTitle(),
                                        modules[i].getDuration(), modules[i].calculatePrice());
                            }
                        }
                    }

                    if(!anyAvailable) {
                        System.out.println("\n  No modules available to add!");
                        continue;
                    }

                    System.out.print("Select a module (number): ");
                    int mIdx;
                    try {
                        mIdx = Integer.parseInt(scanner.nextLine().trim()) - 1;
                    }
                    catch(NumberFormatException e) {
                        System.out.println("  Invalid input.");
                        continue;
                    }
                    if(mIdx < 0 || mIdx >= modules.length) {
                        System.out.println("  Invalid module number.");
                        continue;
                    }
                    if(!isModuleAvailable(mIdx, claimedModules, claimedCourses, moduleCourseIdx)) {
                        System.out.println("  That module (or one of its courses) is already in your cart!");
                        continue;
                    }

                    EduComponent selected = modules[mIdx];
                    String moduleName = modules[mIdx].getTitle();
                    double baseDur = modules[mIdx].getDuration();
                    double basePrice = modules[mIdx].calculatePrice();

                    StringBuilder desc = new StringBuilder();
                    desc.append(String.format("[MODULE] %s", moduleName));
                    desc.append(String.format("\n       Duration: %.2f hrs | Base Price: $%.1f", baseDur, basePrice));


                    
                    System.out.println("\n  --- Add-ons for \"" + moduleName + "\" ---");
                    System.out.print("  Add Practice Question Set (+$10.0)? (y/n): ");
                    boolean hasPractice = scanner.nextLine().trim().equalsIgnoreCase("y");
                    if(hasPractice) {
                        selected = new PracticeSetDecorator(selected);
                        desc.append("\n       + Practice Question Set (+$10.0)");
                    }
                    System.out.print("  Add Live Mentor Support (+$20.0)? (y/n): ");
                    boolean hasMentor = scanner.nextLine().trim().equalsIgnoreCase("y");
                    if(hasMentor) {
                        selected = new LiveMentorDecorator(selected);
                        desc.append("\n       + Live Mentor Support (+$20.0)");
                    }
                    desc.append(String.format("\n       >> Item Total: $%.1f", selected.calculatePrice()));

                    cart.addItem(selected, true);
                    claimedModules.add(mIdx);
                    for(int ci : moduleCourseIdx[mIdx]) {
                        claimedCourses.add(ci);
                    }
                    itemDescs.add(desc.toString());

                    System.out.println("\n  >> \"" + moduleName + "\" added to cart!");

                }
                else if(addType.equals("2")) {
                    System.out.println("\nAvailable Individual Courses:");
                    boolean anyAvailable = false;
                    for(int i=0; i < courses.length; i++) {
                        if(claimedCourses.contains(i)) {
                            System.out.printf("  %d. %s  [ALREADY IN CART]%n",
                                    i+1, courses[i].getTitle());
                        }
                        else {
                            anyAvailable = true;
                            System.out.printf("  %d. %s (%.2f hrs, $%.1f)  [from: %s]%n",
                                    i+1, courses[i].getTitle(),
                                    courses[i].getDuration(), courses[i].calculatePrice(),
                                    modules[courseToModule[i]].getTitle());
                        }
                    }

                    if(!anyAvailable) {
                        System.out.println("\n  All courses are already in your cart!");
                        continue;
                    }

                    System.out.print("Select a course (number): ");
                    int cIdx;
                    try {
                        cIdx = Integer.parseInt(scanner.nextLine().trim()) - 1;
                    }
                    catch(NumberFormatException e) {
                        System.out.println("  Invalid input.");
                        continue;
                    }
                    if(cIdx < 0 || cIdx >= courses.length) {
                        System.out.println("  Invalid course number.");
                        continue;
                    }
                    if(claimedCourses.contains(cIdx)) {
                        System.out.println("  That course is already in your cart!");
                        continue;
                    }

                    String courseName = courses[cIdx].getTitle();
                    double baseDur = courses[cIdx].getDuration();
                    double basePrice = courses[cIdx].calculatePrice();

                    StringBuilder desc = new StringBuilder();
                    desc.append(String.format("[COURSE] %s", courseName));
                    desc.append(String.format("\n       Duration: %.2f hrs | Price: $%.1f", baseDur, basePrice));
                    desc.append(String.format("\n       >> Item Total: $%.1f", basePrice));

                    cart.addItem(courses[cIdx], false);
                    claimedCourses.add(cIdx);
                    itemDescs.add(desc.toString());

                    System.out.println("\n  >> \"" + courseName + "\" added to cart!");

                }
                else {
                    System.out.println("  Invalid choice.");
                    continue;
                }


                showCartDetails(cart, itemDescs);

            }
            else if(mainChoice.equals("2")) {
                if(cart.getItems().isEmpty()) {
                    System.out.println("\n  Your cart is empty! Add items first.");
                    continue;
                }


                showCartDetails(cart, itemDescs);

                System.out.print("\nAre you a student from a developing country? (y/n): ");
                boolean isDevelopingCountry = scanner.nextLine().trim().equalsIgnoreCase("y");

                

                EduComponent order;
                if(cart.getItems().size() == 1) {
                    order = cart.getItems().get(0);
                }
                else {
                    Module combined = new Module("Your Order");
                    for(EduComponent e : cart.getItems()) {
                        combined.addComponent(e);
                    }
                    order = combined;
                }


                int moduleCount = cart.getModuleCount();
                if(moduleCount >= 2) {
                    order = new MultiModuleDiscount(order);
                }
                if(order.getDuration() >= 5.0) {
                    order = new SpecialDiscount(order);
                }
                if(isDevelopingCountry) {
                    order = new DevelopingCountryDiscount(order);
                }


                System.out.println("\n==============================================");
                System.out.println("              ORDER SUMMARY");
                System.out.println("==============================================");
                order.display("");
                System.out.println("----------------------------------------------");
                System.out.printf("FINAL TOTAL PRICE : $%.1f%n", order.calculatePrice());
                System.out.println("==============================================");

                
                cart.clear();
                claimedModules.clear();
                claimedCourses.clear();
                itemDescs.clear();
                System.out.println("\nTransaction complete! Cart has been reset.\n");

            } 
            else {
                System.out.println("  Invalid choice. Please enter 1, 2, or 3.");
            }
        }

        scanner.close();
    }
}
