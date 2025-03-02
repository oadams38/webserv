from urllib.parse import parse_qs
from html import escape
import os

def compareNames(name1, name2):
	words = [name1, "love", name2]
	done = []
	array = []
	size = 0
	i = 0
	while i < len(words[0] + words[1] + words[2]):
		for w in words:
			for n in w:
				if not checkIfDone(n, done):
					x = compareCharNames(n, words)
					if x >= 10:
						array.append(x // 10)
						array.append(x % 10)
					else:
						array.append(x)
					done.append(n)
				size += 1
		i += 1
	while len(array) > 2:
	    array = calculateValues(array)
	if len(array) == 2 and array[0] + array[1] < 6:
	    return array[0] + array[1]
	return array[0] * 10 + array[1]


def calculateValues(array):
	newArray = []
	for i in range(len(array) // 2):
		if array[i] + array[len(array) - i - 1] < 10:
			newArray.append(array[i] + array[len(array) - i - 1])
		else:
			newArray.append((array[i] + array[len(array) - i - 1]) // 10)
			newArray.append((array[i] + array[len(array) - i - 1]) % 10)
	if len(array) % 2 == 1:
		newArray.append(array[len(array) // 2])
	return newArray


def checkIfDone(c, array):
	if len(array) == 0:
		return False
	for item in array:
		if c == item:
		    return True
	return False


def compareCharNames(c, names):
    returnVal = 0
    for name in names:
        for char in name:
            if c == char:
                returnVal += 1
    return returnVal

def createHTMLformat(name1, name2, value):
	
	body = "<!DOCTYPE html>\r\n\
<html lang=\"en\">\r\n\
<head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\
    <title>Send Love to 118 712</title>\r\n\
\r\n\
<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Lemon\" type=\"text/css\">\r\n\
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
            background: url('Heartpng.png') no-repeat center center;\r\n\
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
            <p>" + escape(name1) + "'s Love for "+ escape(name2) +"</p>\r\n\
			<p>.</p>\r\n\
			<p>.</p>\r\n\
			<p>.</p>\r\n\
			<p>.</p>\r\n\
            <p1><i><b>" + str(value) + "%</b></i></p1>\r\n\
        </div>\r\n\
    </div>\r\n\
</body>\r\n\
</html>"
	body.encode('utf-8')
	return body

def main():
	query = os.environ["QUERY_STRING"]
	tab = parse_qs(query)
	value = compareNames(str(tab["name1"][0]), str(tab["name2"][0]))
	if (tab["name1"][0] == "Leo" and tab["name2"][0] == "Julie") or (tab["name2"][0] == "Leo" and tab["name1"][0] == "Julie"):
		value = "miiiiiiiiille"
	with open("test.html", 'w') as file:
		file.write(createHTMLformat(tab["name1"][0], tab["name2"][0], value))
	print(createHTMLformat(tab["name1"][0], tab["name2"][0], value))


if __name__ == "__main__":
    main()