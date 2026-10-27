package io;

public class ReportWriterFactory {

    public enum ReportFormat {
        TXT,
        HTML
    }

    public static ReportWriter createWriter(ReportFormat format) {
        switch (format) {
            case TXT:
                return new TxtReportWriter();
            case HTML:
                return new HtmlReportWriter();
            default:
                throw new IllegalArgumentException("Unsupported report format: " + format);
        }
    }

    public static ReportWriter createWriterForFile(String filePath) {
        if (filePath == null) {
            throw new IllegalArgumentException("File path cannot be null");
        }

        String lowerPath = filePath.toLowerCase();
        if (lowerPath.endsWith(".txt")) {
            return createWriter(ReportFormat.TXT);
        } else if (lowerPath.endsWith(".html") || lowerPath.endsWith(".htm")) {
            return createWriter(ReportFormat.HTML);
        } else {
            throw new IllegalArgumentException("Unsupported file extension. Use .txt or .html");
        }
    }
}
