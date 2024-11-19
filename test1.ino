#include <WiFi.h>
#include <WebServer.h>

// Cấu hình WiFi
const char* ssid = "Yellow";
const char* password = "22062007";

// Khởi tạo WebServer
WebServer server(80);

// UART giao tiếp với Arduino
HardwareSerial Arduino1(1); // UART1 cho Arduino thứ nhất
HardwareSerial Arduino2(2); // UART2 cho Arduino thứ hai

String conveyor1Status = "Unknown";
String conveyor2Status = "Unknown";

void setup() {
   Serial.begin(115200);  // Khởi động Serial Monitor

  // Tạo một điểm truy cập WiFi
  WiFi.softAP(ssid, password);  // Tên và mật khẩu WiFi
  Serial.println("WiFi AP đang khởi tạo...");

  // In ra địa chỉ IP của điểm truy cập WiFi
  Serial.print("Địa chỉ IP của AP: ");
  Serial.println(WiFi.softAPIP());

  // Cấu hình các route cho server
  
  

  server.begin();  // Bắt đầu server
  Serial.println("WebServer đã bắt đầu!");

  // Cấu hình route web
  server.on("/", loginhtml);
  server.on("/login.css", logincss);
  server.on("/styles.css", maincss);
  server.on("/main.html", mainhtml);


  server.begin(); // Bắt đầu server
}

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

// Hàm xử lý trang đăng nhập
void loginhtml() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='vi'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Đăng nhập</title>";
  html += "<link rel='stylesheet' href='/login.css'>"; // Liên kết tới file CSS
  html += "<script>";
  html += "function handleLogin(event) {";
  html += "  event.preventDefault();"; // Ngừng hành động mặc định của form
  html += "  var username = document.getElementById('username').value;";
  html += "  var password = document.getElementById('password').value;";
  
  // Thay đổi tên đăng nhập và mật khẩu ở đây
  String correctUsername = "admin"; // Tên đăng nhập mới
  String correctPassword = "123"; // Mật khẩu mới
  
  html += "  if (username === '" + correctUsername + "' && password === '" + correctPassword + "') {"; // Kiểm tra tên đăng nhập và mật khẩu
  html += "    window.location.href = '/main.html';"; // Chuyển hướng đến trang chính nếu đúng
  html += "  } else {";
  html += "    document.getElementById('errorMessage').style.display = 'block';";
  html += "  }";
  html += "}";
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<div class='login-container'>";
  html += "<h1>Đăng nhập</h1>";
  html += "<form id='loginForm' onsubmit='handleLogin(event)'>"; // Gọi hàm xử lý khi gửi form
  html += "<input type='text' id='username' placeholder='Tên đăng nhập' required>";
  html += "<input type='password' id='password' placeholder='Mật khẩu' required>";
  html += "<button type='submit'>Đăng nhập</button>";
  html += "</form>";
  html += "<p id='errorMessage' style='color: red; display: none;'>Sai tên đăng nhập hoặc mật khẩu!</p>"; // Thông báo lỗi
  html += "</div>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html); // Gửi HTML tới trình duyệt
}



// Hàm xử lý file CSS
void logincss() {
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

// Hàm xử lý file CSS cho trang chính
void maincss() {
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
void mainhtml() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='vi'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Trang chính</title>";
  html += "<link rel='stylesheet' href='/styles.css'>"; // Liên kết tới file CSS
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
  html += "<div class='status'>";
  html += "<h2>Trạng thái băng chuyền</h2>";
  html += "<p>Băng chuyền 1: <span id='status1'>" + conveyor1Status + "</span></p>";
  html += "<p>Băng chuyền 2: <span id='status2'>" + conveyor2Status + "</span></p>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "function updateStatus() {";
  html += "  fetch('/status1').then(response => response.text()).then(data => {";
  html += "    document.getElementById('status1').innerText = data;";
  html += "  });";
  html += "  fetch('/status2').then(response => response.text()).then(data => {";
  html += "    document.getElementById('status2').innerText = data;";
  html += "  });";
  html += "}";
  html += "setInterval(updateStatus, 2000);"; // Cập nhật mỗi 2 giây
  html += "</script>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html); // Gửi HTML tới trình duyệt
}
