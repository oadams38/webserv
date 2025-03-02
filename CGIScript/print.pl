my $input = <STDIN>;
chomp($input);
print "<!DOCTYPE html>\r\n\
<html lang=\"en\">\r\n\
<head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\
    <title>Send Love to 118 712</title>\r\n\
\r\n\
    <style>\r\n\
        body {\r\n\
            margin: 0;\r\n\
            padding: 0;\r\n\
            display: flex;\r\n\
            justify-content: center;\r\n\
            align-items: center;\r\n\
            height: 100vh;\r\n\
        }\r\n\
\r\n\
        .background {\r\n\
            position: absolute;\r\n\
            top: 0;\r\n\
            left: 0;\r\n\
            width: 100%;\r\n\
            height: 100vh;\r\n\
            background: url('printer.jpg') no-repeat center center;\r\n\
            background-size: contain;\r\n\
            background-color: black;\r\n\
        }\r\n\
\r\n\
        .background .text-container {\r\n\
            position: absolute;\r\n\
            top: 50%;\r\n\
            left: 50%;\r\n\
            transform: translate(-50%, -50%);\r\n\
            text-align: center;\r\n\
            color: white;\r\n\
            text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.7);\r\n\
        }\r\n\
\r\n\
        .background .text-container p {\r\n\
            margin: 0; \r\n\
            font-size: 3rem;\r\n\
            font-family: 'Lemon', Arial, sans-serif;\r\n\
        }\r\n\
\r\n\
        .background .text-container p1 {\r\n\
            margin: 0;\r\n\
            color: #e859d2;\r\n\
            font-size: 2rem;\r\n\
            font-family: 'Lemon', Arial, sans-serif;\r\n\
        }\r\n\
    </style>\r\n\
</head>\r\n\
<body>\r\n\
    <div class=\"background\">\r\n\
        <div class=\"text-container\">\r\n\
            <p>Your Desired input is:</p>\r\n\
            <p1><i><b>$input</b></i></p1>\r\n\
        </div>\r\n\
    </div>\r\n\
</body>\r\n\
</html>";