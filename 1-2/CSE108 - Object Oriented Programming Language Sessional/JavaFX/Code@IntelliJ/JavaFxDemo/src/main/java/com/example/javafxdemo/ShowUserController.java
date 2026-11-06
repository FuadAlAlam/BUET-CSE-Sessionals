package com.example.javafxdemo;

import javafx.event.ActionEvent;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class ShowUserController {

    public TextField nameTextField;

    public Label nameLabel;

    public void onChangeNameClick(ActionEvent actionEvent) {
        nameLabel.setText("Hello " + nameTextField.getText());
    }
}
