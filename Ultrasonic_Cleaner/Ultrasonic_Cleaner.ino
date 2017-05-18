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
	webBusy += bool_heater ? "<strong><span style=\"color: #ff0000;\">ON</span></strong></h3>" : "OFF</h3>";
	webBusy += "<a href=\"heat\"><button>TURN ON/OFF</button></a>";
	webBusy += "<h3>Time left: ";
	if ((cleantime / 60) > 60)
	{
		webBusy += String((cleantime / 60) / 60);
		webBusy += " hours ";
		webBusy += String((cleantime / 60) % 60);
	}
	else
	{
		webBusy += String(cleantime / 60);
	}
	webBusy += " minutes and ";
	webBusy += String(cleantime % 60);
	webBusy += " seconds.</h3><p><a href=\"stop\"><button>EMERGENCY STOP</button></a></p></div>";
	webBusy += "&nbsp;<hr /><p style=\"text-align: center;\">";
	webBusy += "<a href=\"https://github.com/m600x/WUC\">Source Code</a></p>";
	server.send(200, "text/html", webBusy);
}

void normal()
{
	String webPage = "";
	webPage += "<div align=\"center\"><h1>Wireless Ultrasonic Cleaner</h1><h3>Heater is ";
	webPage += bool_heater ? "<strong><span style=\"color: #ff0000;\">ON</span></strong></h3>" : "OFF</h3>";
	webPage += "<a href=\"heat\"><button>TURN ON/OFF</button></a>";
	webPage += "<h3>Timing</h3>";
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
	webPage += "<p></p>";
	webPage += "<a href=\"60\"><button>1 hour</button></a> ";
	webPage += "<a href=\"120\"><button>2 hours</button></a> ";
	webPage += "<a href=\"180\"><button>3 hours</button></a> ";
	webPage += "<a href=\"240\"><button>4 hours</button></a> ";
	webPage += "<a href=\"300\"><button>5 hours</button></a> ";
	webPage += "<a href=\"360\"><button>6 hours</button></a> ";
	webPage += "<a href=\"420\"><button>7 hours</button></a> ";
	webPage += "<a href=\"480\"><button>8 hours</button></a> ";
	webPage += "<a href=\"540\"><button>9 hours</button></a> ";
	webPage += "<a href=\"600\"><button>10 hours</button></a> ";
	webPage += "</div>&nbsp;<hr /><p style=\"text-align: center;\">";
	webPage += "<a href=\"https://github.com/m600x/WUC\">Source Code</a></p>";
	server.send(200, "text/html", webPage);
}

void dispatcher(int cltime, int heat, int stop)
{
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
	bool_running ? busy() : normal();
	if (cltime && !bool_running)
		cleaning(cltime);
	if (heat)
	{
		if (!bool_heater && pusher(BTN_HEAT))
			bool_heater = 1;
		else if (bool_heater && pusher(BTN_HEAT))
			bool_heater = 0;
	}
	if (stop)
	{
		if (bool_heater && pusher(BTN_HEAT))
			bool_heater = 0;
		if (bool_running)
			bool_running = 0;
	}
}

void setserver()
{
	server.on("/", [](){ bool_running ? busy() : normal();;});
	server.on("/heat", [](){ dispatcher(0, 1, 0);});
	server.on("/stop", [](){ dispatcher(0, 0, 1);});
	server.on("/1", [](){ dispatcher(1, 0, 0);});
	server.on("/2", [](){ dispatcher(2, 0, 0);});
	server.on("/3", [](){ dispatcher(3, 0, 0);});
	server.on("/4", [](){ dispatcher(4, 0, 0);});
	server.on("/5", [](){ dispatcher(5, 0, 0);});
	server.on("/10", [](){ dispatcher(10, 0, 0);});
	server.on("/20", [](){ dispatcher(20, 0, 0);});
	server.on("/30", [](){ dispatcher(30, 0, 0);});
	server.on("/40", [](){ dispatcher(40, 0, 0);});
	server.on("/50", [](){ dispatcher(50, 0, 0);});
	server.on("/60", [](){ dispatcher(60, 0, 0);});
	server.on("/120", [](){ dispatcher(120, 0, 0);});
	server.on("/180", [](){ dispatcher(180, 0, 0);});
	server.on("/240", [](){ dispatcher(240, 0, 0);});
	server.on("/300", [](){ dispatcher(300, 0, 0);});
	server.on("/360", [](){ dispatcher(360, 0, 0);});
	server.on("/420", [](){ dispatcher(420, 0, 0);});
	server.on("/480", [](){ dispatcher(480, 0, 0);});
	server.on("/540", [](){ dispatcher(540, 0, 0);});
	server.on("/600", [](){ dispatcher(600, 0, 0);});
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
	setserver();
	server.begin();
}

void loop(void)
{
	server.handleClient();
}
