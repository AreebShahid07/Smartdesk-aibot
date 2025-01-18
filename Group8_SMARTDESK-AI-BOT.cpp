#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include "tinyfiledialogs/tinyfiledialogs.h"

using namespace std;
using namespace cpr;
using namespace nlohmann;

class chatbot {
private:
	string url;
	string question;
	json payload;
	Header headers;
	Response response;
	json responseJson;
	string content;
public:
	void func() {
		url = "https://chatgpt-vision1.p.rapidapi.com/gpt4";
		system("cls");
		cout << "Enter your question: ";
		getline(cin, question);
		payload = {
		    {"messages", json::array({
			   {{"role", "user"}, {"content", question}}
		    })},
		    {"web_access", "false"}
		};

		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "chatgpt-vision1.p.rapidapi.com"}
		};

		response = Post(Url{ url }, Body{ payload.dump() }, headers);
		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response: " << response.text << endl;
			return;
		}

		responseJson = json::parse(response.text);
		content = responseJson["result"];
		cout << "Answer: " << content << endl;
	}
};

class image_generate {
private:
	string url;
	string text;
	json payload;
	Header headers;
	Response response;
	json responseJson;
	string image_url;
	Response imageResponse;
public:
	void func() {
		url = "https://ai-image-generator14.p.rapidapi.com/";
		system("cls");
		cout << "Enter the text of which you wanna create image: ";
		getline(cin, text);
		payload = {
		    {"jsonBody", {
			   {"function_name", "image_generator"},
			   {"type", "image_generation"},
			   {"query", text},
			   {"output_type", "png"}
		    }}
		};

		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "ai-image-generator14.p.rapidapi.com"}
		};

		response = Post(Url{ url }, Body{ payload.dump() }, headers);
		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			return;
		}

		responseJson = json::parse(response.text);
		image_url = responseJson["message"]["output_png"];

		imageResponse = Get(Url{ image_url });
		if (imageResponse.status_code != 200) {
			cerr << "Failed to download image. Status code: " << imageResponse.status_code << endl;
			return;
		}

		ofstream outputFile("outputimg.png", ios::binary);
		outputFile.write(imageResponse.text.c_str(), imageResponse.text.size());
		outputFile.close();

		cout << "Image saved as output.png" << endl;
		system("outputimg.png");
	}
};

class text_extract {
private:
	string url;
	const char* filePath;
	Header headers;
	Response response;
	json responseJson;
	string content;
public:
	void func() {
		url = "https://chatgpt-vision1.p.rapidapi.com/ocrvisionform";
		system("cls");
		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "chatgpt-vision1.p.rapidapi.com"}
		};

		filePath = tinyfd_openFileDialog("Select a File", "", 0,0,0, 0);
		if (!filePath) {
			cout << "No file selected!" << endl;
			return;
		}
		cout << "Selected file: " << filePath << endl;

		response = Post(
			Url{ url },
			headers,
			Multipart{ {"file", File{filePath}} }
		);

		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response: " << response.text << endl;
			return;
		}

		responseJson = json::parse(response.text);
		content = responseJson["result"];
		cout << "Answer: " << content << endl;
	}
};

class background_remove {
private:
	string url;
	const char* filePath;
	Header headers;
	Response response;
public:
	void func() {
		system("cls");
		url = "https://ai-background-remover.p.rapidapi.com/image/matte/v1";
		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "ai-background-remover.p.rapidapi.com"}
		};

		filePath = tinyfd_openFileDialog("Select a File", "", 0, 0,0, 0);
		if (!filePath) {
			cout << "No file selected!" << endl;
			return;
		}
		cout << "Selected file: " << filePath << endl;

		response = Post(
			Url{ url },
			headers,
			Multipart{ {"image", File{filePath}} }
		);

		if (response.status_code != 200) {
			cerr << "Error: " << response.status_code << " - " << response.text << endl;
			return;
		}

		ofstream output_file("output.png", ios::binary);
		if (!output_file.is_open()) {
			cerr << "Failed to open output file" << endl;
			return;
		}

		output_file.write(response.text.c_str(), response.text.size());
		output_file.close();

		cout << "Background removed as output.png" << endl;
		system("output.png");
	}
};

class qrcode_generate {
private:
	string url;
	string text;
	Parameters params;
	Response response;
public:
	void func() {
		system("cls");
		url = "https://api.qrserver.com/v1/create-qr-code/";
		cout << "Enter text or link you want to create QR code for: ";
		getline(cin, text);

		params = { {"data", text}, {"size", "300x300"} };
		response = Get(Url{ url }, params);

		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response body: " << response.text << endl;
			return;
		}

		ofstream outputFile("QR.png", ios::binary);
		outputFile.write(response.text.c_str(), response.text.size());
		outputFile.close();

		cout << "QR code saved as output.png" << endl;
		system("QR.png");
	}
};

class audio_generate {
private:
	string url;
	string text;
	Parameters params;
	Header headers;
	Response response;
public:
	void func() {
		system("cls");
		url = "https://text-to-speach-english.p.rapidapi.com/makevoice";
		cout << "Write text to convert to AI voice: ";
		getline(cin, text);
		params = { {"text", text} };
		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "text-to-speach-english.p.rapidapi.com"}
		};

		response = Get(Url{ url }, params, headers);
		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			return;
		}

		ofstream outputFile("audio.mp3", ios::binary);
		outputFile.write(response.text.c_str(), response.text.size());
		outputFile.close();

		cout << "Audio saved as output.mp3" << endl;
		system("audio.mp3");
	}
};

class sentiment_analyze {
private:
	string url;
	string text;
	Parameters params;
	Header headers;
	Response response;
	json responseJson;
	string sentiment;
public:
	void func() {
		url = "https://sentiment-analyzer3.p.rapidapi.com/Sentiment";
		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "sentiment-analyzer3.p.rapidapi.com"}
		};

		system("cls");
		cout << "Enter your text: ";
		getline(cin, text);

		params = { {"text", text} };
		response = Get(Url{ url }, params, headers);

		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response: " << response.text << endl;
			return;
		}

		responseJson = json::parse(response.text);
		sentiment = responseJson["sentiment"];
		cout << "Sentiment: " << sentiment << endl;
	}
};

class url_shorten {
private:
	string url;
	string long_url;
	Response response;
public:
	void func() {
		url = "http://tinyurl.com/api-create.php";
		system("cls");
		cout << "Enter the URL to shorten: ";
		getline(cin, long_url);

		response = Get(Url{ url }, Parameters{ {"url", long_url} });
		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response: " << response.text << endl;
			return;
		}

		cout << "Shortened URL: " << response.text << endl;
	}
};
		

class find_song {
private:
	string url;
	Header headers;
	const char* filePath;
	Response response;
	json responseJson;
	string content;
	string genre;
	string singer;
public:
	void func() {
		system("cls");
		url = "https://shazam-api6.p.rapidapi.com/shazam/recognize/";
		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "shazam-api6.p.rapidapi.com"}
		};

		filePath = tinyfd_openFileDialog("Select a File", "", 0, 0, 0, 0);
		if (!filePath) {
			cout << "No file selected!" << endl;
			return;
		}
		cout << "Selected file: " << filePath << endl;

		response = Post(
			Url{ url },
			headers,
			Multipart{ {"upload_file", File{filePath}} }
		);

		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			return;
		}

		responseJson = json::parse(response.text);
		content = responseJson["track"]["title"];
		singer = responseJson["track"]["subtitle"];
		genre = responseJson["track"]["genres"]["primary"];
		cout << "Song Title: " << content << endl;
		cout << "Singer Name: " << singer << endl;
		cout << "Genre Name: " << genre << endl;
	}
};

class colorize_image {
private:
	string url;
	string photo;
	json payload;
	Header headers;
	Response response;
public:
	void func() {
		system("cls");

		url = "https://ai-picture-colorizer1.p.rapidapi.com/colorize/v1/";
		cout << "Enter URL of Black and White Image: ";
		getline(cin, photo);

		headers = {
		    {"x-rapidapi-key", "9ec364a01dmsh788ebdab04bbf6bp181b84jsn7d07939b6165"},
		    {"x-rapidapi-host", "ai-picture-colorizer1.p.rapidapi.com"}
		};

		response = Post(Url{ url }, Multipart{ {"image_url", photo}}, headers);

		if (response.status_code != 200) {
			cerr << "API request failed. Status code: " << response.status_code << endl;
			cerr << "Response: " << response.text << endl;
			return;
		}

		ofstream outputFile("clroutput.png", ios::binary);
		outputFile.write(response.text.c_str(), response.text.size());
		outputFile.close();

		cout << "Colorized image saved as clroutput.png" << endl;
		system("clroutput.png");
	}
};

int main() {
	chatbot chat;
	image_generate image;
	text_extract text;
	background_remove background;
	qrcode_generate qrcode;
	audio_generate audio;
	sentiment_analyze sentiment;
	url_shorten url;
	find_song song;
	colorize_image color;
	int choice;

	while (true) {
		system("cls");
		cout << "=====================================================" << endl;
		cout << "|                  SMARTDESK AI BOT                 |" << endl;
		cout << "=====================================================" << endl;
		cout << "|  1.   Chatbot                                     |" << endl;
		cout << "|  2.   Image Generator                             |" << endl;
		cout << "|  3.   Text Extractor                              |" << endl;
		cout << "|  4.   Background Remover                          |" << endl;
		cout << "|  5.   QR code Generator                           |" << endl;
		cout << "|  6.   Audio Generator                             |" << endl;
		cout << "|  7.   Sentiment Analyzer                          |" << endl;
		cout << "|  8.   URL Shortener                               |" << endl;
		cout << "|  9.   Song Finder                                 |" << endl;
		cout << "| 10.   Image Colorizer                             |" << endl;
		cout << "| 11.   Exit                                        |" << endl;
		cout << "=====================================================" << endl;
		cout << "=====================================================" << endl;
		cout << "|  ENTER YOUR CHOICE (1 - 11)                       |" << endl;
		cout << "=====================================================" << endl;

		cin >> choice;
		cin.clear(); 
		cin.ignore();

		switch (choice) {
		case 1:
			chat.func();
			system("pause");
			break;
		case 2:
			image.func();
			system("pause");
			break;
		case 3:
			text.func();
			system("pause");
			break;
		case 4:
			background.func();
			system("pause");
			break;
		case 5:
			qrcode.func();
			system("pause");
			break;
		case 6:
			audio.func();
			system("pause");
			break;
		case 7:
			sentiment.func();
			system("pause");
			break;
		case 8:
			url.func();
			system("pause");
			break;
		case 9:
			song.func();
			system("pause");
			break;
		case 10:
			color.func();
			system("pause");
			break;
		case 11:
			cout << "Thank you for using SMARTDESK AI BOT!" << endl;
			system("pause");
			return 0;
		default:
			cout << "Invalid choice! Please enter a number between 1 and 11." << endl;
			system("pause");
		}
	}

	return 0;
}
