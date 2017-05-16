/*
** Wireless Ultrasonic Cleaner - WiFi addon
**
** For more informations please refer to :
** https://github.com/m600x/WUC
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char*				ssid = "yourssid";
const char*				password = "yourpassword";

#define		BTN_ON		D0
#define		BTN_OFF		D5
#define		BTN_SET		D6
#define		BTN_HEAT	D7

ESP8266WebServer		server(80);
MDNSResponder			mdns;
int						bool_heater = 0;
int						bool_running = 0;
int						cleantime = 0;

int pusher(int pin)
{
	digitalWrite(pin, HIGH);
	delay(20);
	digitalWrite(pin, LOW);
	return (1);
}

void cleaning(int cltime)
{
	int		st_time = 0;
	int		interval = 0;

	bool_running = 1;
	cleantime = cltime * 60;
	pusher(BTN_ON);
	while (cltime && bool_running)
	{
		st_time = millis() + 60000 - 20;
		interval = millis();
		while ((millis() < st_time) && bool_running)
		{
			if (((millis() - interval) > 1000) && (interval = millis()))
				cleantime--;
			server.handleClient();
		}
		cltime--;
		pusher(BTN_SET);
	}
	pusher(BTN_OFF);
	bool_running = 0;
	if (bool_heater && pusher(BTN_HEAT))
		bool_heater = 0;
}

void busy()
{
	String webBusy = "";
	webBusy += "<div align=\"center\"><h1>Cleaning in progress</h1><h3>Heater is currently ";
	webBusy += bool_heater ? "ON</h3>" : "OFF</h3>";
	webBusy += "<a href=\"heat1\"><button>ON</button></a> <a href=\"heat0\"><button>OFF</button></a>";
	webBusy += "<h3>Time left: ";
	webBusy += String(cleantime / 60);
	webBusy += " minutes and ";
	webBusy += String(cleantime % 60);
	webBusy += " seconds.</h3><p><a href=\"emergency\"><button>EMERGENCY STOP</button></a></p></div>";
	server.send(200, "text/html", webBusy);
}

void normal()
{
	String webPage = "";
	webPage += "<div align=\"center\"><h1>UltraSonic Cleaner</h1><h3>Heater is currently ";
	webPage += bool_heater ? "ON</h3>" : "OFF</h3>";
	webPage += "<a href=\"heat1\"><button>ON</button></a> <a href=\"heat0\"><button>OFF</button></a>";
	webPage += "<h3>Timer in minutes</h3>";
	webPage += "<a href=\"1\"><button>1:00</button></a> ";
	webPage += "<a href=\"2\"><button>2:00</button></a> ";
	webPage += "<a href=\"3\"><button>3:00</button></a> ";
	webPage += "<a href=\"4\"><button>4:00</button></a> ";
	webPage += "<a href=\"5\"><button>5:00</button></a> ";
	webPage += "<a href=\"10\"><button>10:00</button></a> ";
	webPage += "<a href=\"20\"><button>20:00</button></a> ";
	webPage += "<a href=\"30\"><button>30:00</button></a> ";
	webPage += "<a href=\"40\"><button>40:00</button></a> ";
	webPage += "<a href=\"50\"><button>50:00</button></a> ";
	webPage += "<h3>Timer in hours</h3>";
	webPage += "<a href=\"60\"><button>1:00</button></a> ";
	webPage += "<a href=\"90\"><button>1:30</button></a> ";
	webPage += "<a href=\"120\"><button>2:00</button></a> ";
	webPage += "<a href=\"150\"><button>2:30</button></a> ";
	webPage += "<a href=\"180\"><button>3:00</button></a> ";
	webPage += "<a href=\"210\"><button>3:30</button></a> ";
	webPage += "<a href=\"240\"><button>4:00</button></a> ";
	webPage += "<a href=\"270\"><button>4:30</button></a> ";
	webPage += "<a href=\"300\"><button>5:00</button></a> ";
	webPage += "</div>";
	server.send(200, "text/html", webPage);
}

void dispatcher(int cltime)
{
	bool_running ? busy() : normal();
	if (!bool_running)
		cleaning(cltime);
}

void setup()
{
	pinMode(BTN_ON, OUTPUT);
	pinMode(BTN_OFF, OUTPUT);
	pinMode(BTN_SET, OUTPUT);
	pinMode(BTN_HEAT, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(BTN_ON, LOW);
	digitalWrite(BTN_OFF, LOW);
	digitalWrite(BTN_SET, LOW);
	digitalWrite(BTN_HEAT, LOW);
	pusher(BTN_OFF);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
		delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	mdns.begin("esp8266", WiFi.localIP());
	server.on("/", [](){
		bool_running ? busy() : normal();
	});
	server.on("/heat1", [](){
		bool_running ? busy() : normal();
		if (!bool_heater && pusher(BTN_HEAT))
			bool_heater = 1;
	});
	server.on("/heat0", [](){
		bool_running ? busy() : normal();
		if (bool_heater && pusher(BTN_HEAT))
			bool_heater = 0;
	});
	server.on("/emergency", [](){
		if (bool_heater && pusher(BTN_HEAT))
			bool_heater = 0;
		if (bool_running)
			bool_running = 0;
		bool_running ? busy() : normal();
	});
	server.on("/1", [](){ dispatcher(1);});
	server.on("/2", [](){ dispatcher(2);});
	server.on("/3", [](){ dispatcher(3);});
	server.on("/4", [](){ dispatcher(4);});
	server.on("/5", [](){ dispatcher(5);});
	server.on("/10", [](){ dispatcher(10);});
	server.on("/20", [](){ dispatcher(20);});
	server.on("/30", [](){ dispatcher(30);});
	server.on("/40", [](){ dispatcher(40);});
	server.on("/50", [](){ dispatcher(50);});
	server.on("/60", [](){ dispatcher(60);});
	server.on("/90", [](){ dispatcher(90);});
	server.on("/120", [](){ dispatcher(120);});
	server.on("/150", [](){ dispatcher(150);});
	server.on("/180", [](){ dispatcher(180);});
	server.on("/210", [](){ dispatcher(210);});
	server.on("/240", [](){ dispatcher(240);});
	server.on("/270", [](){ dispatcher(270);});
	server.on("/300", [](){ dispatcher(300);});
	server.begin();
}

void loop(void)
{
	server.handleClient();
}
