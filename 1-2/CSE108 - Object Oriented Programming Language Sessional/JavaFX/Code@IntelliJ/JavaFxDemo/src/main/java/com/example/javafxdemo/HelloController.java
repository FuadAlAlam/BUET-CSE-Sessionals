package com.example.javafxdemo;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;

import java.io.IOException;

public class HelloController {

    HelloApplication application;

    public HelloApplication getApplication() {
        return application;
    }

    public void setApplication(HelloApplication application) {
        this.application = application;
    }

    @FXML
    private Label welcomeText;

    public void onHelloButtonClick(ActionEvent actionEvent) {
        welcomeText.setText("Welcome to JavaFX Application!");
    }

    public void onByeClick(ActionEvent actionEvent) {
        welcomeText.setText("Bye!");
    }

    public void onNextSceneClick(ActionEvent actionEvent) throws IOException {
        application.goToShowUser();
    }
}