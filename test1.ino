#include <WiFi.h>
#include <WebServer.h>

// Cấu hình WiFi
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Khởi tạo WebServer
WebServer server(80);

// UART giao tiếp với Arduino
HardwareSerial Arduino1(1); // UART1 cho Arduino thứ nhất
HardwareSerial Arduino2(2); // UART2 cho Arduino thứ hai

String conveyor1Status = "Unknown";
String conveyor2Status = "Unknown";

void setup() {
  Serial.begin(115200); // Debug
  Arduino1.begin(9600, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17
  Arduino2.begin(9600, SERIAL_8N1, 4, 5);   // RX = GPIO4, TX = GPIO5

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  // Cấu hình route web
  server.on("/", handleRoot);
  server.begin();
}
server.on("/login.css", handleCSS); // Route xử lý file CSS

void loop() {
  server.handleClient(); // Xử lý yêu cầu web

  // Đọc trạng thái từ Arduino 1
  if (Arduino1.available()) {
    conveyor1Status = Arduino1.readStringUntil('\n');
  }

  // Đọc trạng thái từ Arduino 2
  if (Arduino2.available()) {
    conveyor2Status = Arduino2.readStringUntil('\n');
  }
}

// Hàm xử lý trang web chính
  void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='vi'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Đăng nhập</title>";
  html += "<link rel='stylesheet' href='/login.css'>"; // Liên kết tới file CSS
  html += "</head>";
  html += "<body>";
  html += "<div class='login-container'>";
  html += "<h1>Đăng nhập</h1>";
  html += "<form id='loginForm'>";
  html += "<input type='text' id='username' placeholder='Tên đăng nhập' required>";
  html += "<input type='password' id='password' placeholder='Mật khẩu' required>";
  html += "<button type='submit'>Đăng nhập</button>";
  html += "</form>";
  html += "<p id='errorMessage' style='color: red; display: none;'></p>"; // Thông báo lỗi
  html += "</div>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html); // Gửi HTML tới trình duyệt
}
void handleCSS() {
  String css = "body { font-family: 'Arial', sans-serif; background: linear-gradient(to right, #6a11cb, #2575fc); color: #333; margin: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }";
  css += ".login-container { background-color: #fff; box-shadow: 0 4px 20px rgba(0, 0, 0, 0.2); border-radius: 10px; width: 350px; padding: 30px; text-align: center; }";
  css += "h1 { margin-bottom: 20px; color: #6a11cb; }";
  css += "input[type='text'], input[type='password'] { width: 100%; padding: 12px; margin: 10px 0; border: 1px solid #ccc; border-radius: 5px; transition: border-color 0.3s; }";
  css += "input[type='text']:focus, input[type='password']:focus { border-color: #6a11cb; outline: none; }";
  css += "button { background-color: #6a11cb; color: white; border: none; padding: 12px 15px; border-radius: 5px; cursor: pointer; transition: background-color 0.3s, transform 0.3s; }";
  css += "button:hover { background-color: #2575fc; transform: translateY(-2px); }";
  css += "#errorMessage { margin-top: 10px; font-size: 14px; }";
  
  server.send(200, "text/css", css); // Gửi nội dung CSS
}


void handleIndex() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='vi'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Trang chính</title>";
  html += "<link rel='stylesheet' href='/styles.css'>"; // Liên kết file CSS
  html += "</head>";
  html += "<body>";
  html += "<header>";
  html += "<nav>";
  html += "<ul>";
  html += "<li><a href='/control.html'>Điều khiển băng chuyền</a></li>";
  html += "<li><a href='/history.html'>Xem lịch sử</a></li>";
  html += "<li><a href='/statistics.html'>Thống kê sản phẩm</a></li>";
  html += "<li><a href='/index.html' onclick='logout()'>Đăng xuất</a></li>";
  html += "</ul>";
  html += "</nav>";
  html += "</header>";
  html += "<div class='main-container'>";
  html += "<h1>Chào mừng đến với hệ thống điều khiển băng chuyền IoT!</h1>";
  html += "<p>Chọn một trong các chức năng ở trên để bắt đầu.</p>";
  html += "</div>";
  html += "<script src='/script.js'></script>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html); // Gửi HTML về trình duyệt
}
void handleCSS() {
  String css = "body { font-family: 'Arial', sans-serif; margin: 0; padding: 0; background-color: #f4f4f9; }";
  css += "header { background-color: #6a11cb; color: white; padding: 10px 0; }";
  css += "nav ul { list-style: none; padding: 0; margin: 0; display: flex; justify-content: center; }";
  css += "nav ul li { margin: 0 15px; }";
  css += "nav ul li a { color: white; text-decoration: none; font-weight: bold; }";
  css += "nav ul li a:hover { text-decoration: underline; }";
  css += ".main-container { text-align: center; padding: 20px; }";
  css += ".main-container h1 { color: #6a11cb; }";
  server.send(200, "text/css", css); // Gửi CSS về trình duyệt
}
void handleJS() {
  String js = "function logout() {";
  js += "  alert('Bạn đã đăng xuất thành công!');";
  js += "}";
  server.send(200, "application/javascript", js); // Gửi JavaScript về trình duyệt
}
