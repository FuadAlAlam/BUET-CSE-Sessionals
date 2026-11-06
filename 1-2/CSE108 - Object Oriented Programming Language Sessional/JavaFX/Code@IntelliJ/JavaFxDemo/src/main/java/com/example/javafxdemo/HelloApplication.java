package com.example.javafxdemo;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class HelloApplication extends Application {
    Stage stage;

    @Override
    public void start(Stage stage) throws IOException {
        this.stage = stage;

        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 320, 240);

        HelloController helloController = fxmlLoader.getController();
        helloController.setApplication(this);

        stage.setTitle("Hello!");
        stage.setScene(scene);
        stage.show();
    }

    public void goToShowUser() throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("show-user.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 440, 336);
        stage.setTitle("User Title");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}