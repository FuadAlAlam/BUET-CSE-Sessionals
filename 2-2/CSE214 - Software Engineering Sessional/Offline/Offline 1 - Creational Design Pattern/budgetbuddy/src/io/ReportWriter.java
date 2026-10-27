package io;

import service.ExpenseRepository;

import java.io.IOException;
import java.time.LocalDate;
import java.time.YearMonth;
import java.time.format.DateTimeFormatter;

public abstract class ReportWriter {

    protected final DateTimeFormatter dateFormatter;
    protected final DateTimeFormatter monthFormatter;

    protected ReportWriter() {
        this.dateFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        this.monthFormatter = DateTimeFormatter.ofPattern("yyyy-MM");
    }

    public abstract void writeReport(String filePath, ExpenseRepository repository) throws IOException;

    protected String formatDate(LocalDate date) {
        return date.format(dateFormatter);
    }

    protected String formatMonth(YearMonth month) {
        return month.format(monthFormatter);
    }

    protected String formatAmount(double amount) {
        return String.format("%.2f", amount);
    }
}
