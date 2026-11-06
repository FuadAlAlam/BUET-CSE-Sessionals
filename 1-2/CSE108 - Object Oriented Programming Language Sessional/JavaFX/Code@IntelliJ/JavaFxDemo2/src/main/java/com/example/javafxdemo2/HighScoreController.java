package com.example.javafxdemo2;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class HighScoreController {
    Stage stage;

    public Stage getStage() {
        return stage;
    }

    public void setStage(Stage stage) {
        this.stage = stage;
    }

    @FXML
    public void onBackButtonClick(ActionEvent actionEvent) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(MenuApplication.class.getResource("menu-menu-view.fxml"));

        Scene scene = new Scene(fxmlLoader.load(), 320, 240);

        MainMenuController controller = fxmlLoader.getController();
        controller.setStage(stage);

        stage.setTitle("Main Menu");
        stage.setScene(scene);
        stage.show();
    }
}
