#include <iostream>
#include <string>
#include <map>
#include <mysql/mysql.h>
#include <cstdlib>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>

using namespace std;

// URL decode helper
string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
            result += static_cast<char>(value);
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

// Parse POST data from CGI
map<string, string> parsePostData() {
    map<string, string> data;
    char* contentLength = getenv("CONTENT_LENGTH");

    if (contentLength) {
        int length = atoi(contentLength);
        if (length > 0) {
            string postData;
            postData.resize(length);
            cin.read(&postData[0], length);

            // Parse key=value&key=value format
            stringstream ss(postData);
            string pair;
            while (getline(ss, pair, '&')) {
                size_t pos = pair.find('=');
                if (pos != string::npos) {
                    string key = pair.substr(0, pos);
                    string value = pair.substr(pos + 1);
                    data[key] = urlDecode(value);
                }
            }
        }
    }
    return data;
}

// Simple SHA-256 hash (for MVP - use bcrypt in production)
string hashPassword(const string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main() {
    // Check if POST or GET
    char* requestMethod = getenv("REQUEST_METHOD");

    // ===== GET: Themed login page =====
    if (!requestMethod || string(requestMethod) != "POST") {
        cout << "Content-Type: text/html\n\n";
        cout
        << "<!DOCTYPE html>\n"
           "<html lang='en'>\n"
           "<head>\n"
           "  <meta charset='utf-8'>\n"
           "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
           "  <title>Login · Team Elevate</title>\n"
           "  <style>\n"
           "    :root{--ink:#0f172a;--muted:#475569;--bg:#f6f7fb;--card:#ffffff;--brand:#4f46e5}\n"
           "    *{box-sizing:border-box}\n"
           "    html,body{margin:0;padding:0}\n"
           "    body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;color:var(--ink);background:var(--bg);line-height:1.5}\n"
           "    header{background:linear-gradient(135deg,#111827,#1f2937 50%,#111827);color:#fff}\n"
           "    .wrap{max-width:1024px;margin:0 auto;padding:0 20px}\n"
           "    .nav{display:flex;align-items:center;justify-content:space-between;padding:18px 0}\n"
           "    .brand{display:flex;gap:10px;align-items:center}\n"
           "    .logo{width:36px;height:36px;border-radius:10px;background:linear-gradient(135deg,var(--brand),#7c3aed)}\n"
           "    .brand h1{font-size:18px;margin:0;font-weight:700;letter-spacing:.3px}\n"
           "    .links a{color:#e5e7eb;text-decoration:none;margin-left:14px;font-weight:600}\n"
           "    .links a:hover{color:#fff;text-decoration:underline}\n"
           "    main{padding:28px 0}\n"
           "    .center{display:grid;place-items:center}\n"
           "    .card{background:var(--card);border-radius:16px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:22px;width:100%;max-width:440px}\n"
           "    .card h2{margin:0 0 4px;font-size:24px}\n"
           "    .sub{color:var(--muted);margin:0 0 16px;font-size:14px}\n"
           "    label{display:block;font-weight:600;margin:12px 0 6px}\n"
           "    input{width:100%;padding:10px 12px;border:1px solid #e5e7eb;border-radius:10px;background:#fff}\n"
           "    input:focus{outline:none;border-color:var(--brand);box-shadow:0 0 0 3px rgba(79,70,229,.15)}\n"
           "    .actions{margin-top:16px}\n"
           "    .btn{background:var(--brand);color:#fff;border:none;border-radius:14px;padding:14px 16px;font-weight:800;cursor:pointer;width:100%;display:block;text-align:center}\n"
           "    .btn:hover{filter:brightness(1.05)}\n"
           "    .muted{color:var(--muted)}\n"
           "    .helper{margin-top:12px;font-size:14px}\n"
           "    a{color:var(--brand);text-decoration:none}\n"
           "    a:hover{text-decoration:underline}\n"
           "  </style>\n"
           "</head>\n"
           "<body>\n"
           "  <header>\n"
           "    <div class='wrap'>\n"
           "      <div class='nav'>\n"
           "        <div class='brand'>\n"
           "          <span class='logo' aria-hidden='true'></span>\n"
           "          <h1>Team Elevate Auctions</h1>\n"
           "        </div>\n"
           "        <nav class='links'>\n"
           "          <a href='/~elevate/cgi/index.cgi'>Home</a>\n"
           "          <a href='/~elevate/cgi/register.cgi'>Register</a>\n"
           "        </nav>\n"
           "      </div>\n"
           "    </div>\n"
           "  </header>\n"
           "  <main class='center'>\n"
           "    <div class='card' role='form' aria-labelledby='login-title'>\n"
           "      <h2 id='login-title'>Welcome back</h2>\n"
           "      <p class='sub'>Log in to continue bidding.</p>\n"
           "      <form method='post' action='/~elevate/cgi/login.cgi' novalidate>\n"
           "        <label for='email'>Email</label>\n"
           "        <input id='email' name='email' type='email' maxlength='100' required autocomplete='email'>\n"
           "        <label for='password'>Password</label>\n"
           "        <input id='password' name='password' type='password' minlength='8' required autocomplete='current-password'>\n"
           "        <div class='actions'>\n"
           "          <button class='btn' type='submit'>Log In</button>\n"
           "        </div>\n"
           "      </form>\n"
           "      <p class='helper muted'>Don't have an account? <a href='/~elevate/cgi/register.cgi'>Create one</a>.</p>\n"
           "    </div>\n"
           "  </main>\n"
           "</body>\n"
           "</html>\n";
        return 0;
    }

    // ===== POST: authenticate (logic unchanged) =====
    map<string, string> postData = parsePostData();
    string email = postData["email"];
    string password = postData["password"];

    // Basic validation
    if (email.empty() || password.empty()) {
        cout << "Content-Type: text/html\n\n";
        cout << "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>Error</title>"
                "<style>body{font-family:system-ui,Segoe UI,Roboto,Arial,sans-serif;max-width:560px;margin:48px auto;padding:0 16px}"
                ".card{background:#fff;border-radius:14px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:18px}"
                ".btn{display:inline-block;background:#4f46e5;color:#fff;text-decoration:none;font-weight:800;padding:12px 16px;border-radius:14px}</style>"
                "</head><body><div class='card'><h1>Missing fields</h1><p>Email and password are required.</p>"
                "<p><a class='btn' href='/~elevate/cgi/login.cgi'>Go back</a></p></div></body></html>";
        return 1;
    }

    // Connect to MariaDB
    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL) {
        cout << "Content-Type: text/html\n\n";
        cout << "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>Database Error</title>"
                "<style>body{font-family:system-ui,Segoe UI,Roboto,Arial,sans-serif;max-width:560px;margin:48px auto;padding:0 16px}"
                ".card{background:#fff;border-radius:14px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:18px}</style>"
                "</head><body><div class='card'><h1>Database Error</h1><p>Failed to initialize MySQL.</p></div></body></html>";
        return 1;
    }

    const char* host = "localhost";
    const char* user = "cs370_section2_elevate";
    const char* pass = "etavele_004";
    const char* db   = "cs370_section2_elevate";

    if (mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0) == NULL) {
        cout << "Content-Type: text/html\n\n";
        cout << "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>Database Connection Error</title>"
                "<style>body{font-family:system-ui,Segoe UI,Roboto,Arial,sans-serif;max-width:560px;margin:48px auto;padding:0 16px}"
                ".card{background:#fff;border-radius:14px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:18px}</style>"
                "</head><body><div class='card'><h1>Database Connection Error</h1><p>";
        cout << mysql_error(conn);
        cout << "</p></div></body></html>";
        mysql_close(conn);
        return 1;
    }

    // Hash the password for comparison
    string hashedPassword = hashPassword(password);

    // Prepare query (using mysql_real_escape_string to prevent SQL injection)
    char escapedEmail[201];
    mysql_real_escape_string(conn, escapedEmail, email.c_str(), email.length());

    char escapedHash[513];
    mysql_real_escape_string(conn, escapedHash, hashedPassword.c_str(), hashedPassword.length());

    string query = "SELECT user_id, user_email FROM users WHERE user_email='";
    query += escapedEmail;
    query += "' AND password_hash='";
    query += escapedHash;
    query += "'";

    // Execute query
    if (mysql_query(conn, query.c_str())) {
        cout << "Content-Type: text/html\n\n";
        cout << "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>Query Error</title>"
                "<style>body{font-family:system-ui,Segoe UI,Roboto,Arial,sans-serif;max-width:560px;margin:48px auto;padding:0 16px}"
                ".card{background:#fff;border-radius:14px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:18px}</style>"
                "</head><body><div class='card'><h1>Query Error</h1><p>";
        cout << mysql_error(conn);
        cout << "</p></div></body></html>";
        mysql_close(conn);
        return 1;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    // ===== SUCCESS =====
    if (row) {
        string userId = row[0];
        cout << "Content-Type: text/html\n";
        cout << "Set-Cookie: user_id=" << userId << "; Path=/; HttpOnly\n";
        cout << "Set-Cookie: user_email=" << email << "; Path=/\n\n";

        // Themed success page with AUTO redirect + BUTTON
        cout
        << "<!DOCTYPE html>\n"
           "<html lang='en'>\n"
           "<head>\n"
           "  <meta charset='utf-8'>\n"
           "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
           "  <title>Welcome · Team Elevate</title>\n"
           "  <meta http-equiv='refresh' content='2; url=/~elevate/cgi/index.cgi'>\n"
           "  <script>window.addEventListener('DOMContentLoaded',()=>{setTimeout(()=>{location.replace('/~elevate/cgi/index.cgi');},1500);});</script>\n"
           "  <style>\n"
           "    :root{--ink:#0f172a;--muted:#475569;--bg:#f6f7fb;--card:#ffffff;--brand:#4f46e5}\n"
           "    *{box-sizing:border-box}\n"
           "    html,body{margin:0;padding:0}\n"
           "    body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;color:var(--ink);background:var(--bg);line-height:1.5}\n"
           "    header{background:linear-gradient(135deg,#111827,#1f2937 50%,#111827);color:#fff}\n"
           "    .wrap{max-width:1024px;margin:0 auto;padding:0 20px}\n"
           "    .nav{display:flex;align-items:center;justify-content:space-between;padding:14px 0}\n"
           "    .brand{display:flex;gap:10px;align-items:center}\n"
           "    .logo{width:32px;height:32px;border-radius:10px;background:linear-gradient(135deg,var(--brand),#7c3aed)}\n"
           "    .brand h1{font-size:17px;margin:0;font-weight:700;letter-spacing:.3px}\n"
           "    .links a{color:#e5e7eb;text-decoration:none;margin-left:12px;font-weight:600}\n"
           "    .links a:hover{color:#fff;text-decoration:underline}\n"
           "    main{padding:22px 0}\n"
           "    .center{display:grid;place-items:center}\n"
           "    .card{background:var(--card);border-radius:16px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:20px;width:100%;max-width:560px;text-align:center}\n"
           "    .title{margin:6px 0 4px;font-size:24px}\n"
           "    .muted{color:var(--muted)}\n"
           "    .btn{display:block;margin-top:14px;padding:14px 16px;border-radius:14px;background:var(--brand);color:#fff;font-weight:800;text-decoration:none}\n"
           "    .btn:hover{filter:brightness(1.05)}\n"
           "    .small{font-size:13px;color:var(--muted);margin-top:8px}\n"
           "  </style>\n"
           "</head>\n"
           "<body>\n"
           "  <header>\n"
           "    <div class='wrap'>\n"
           "      <div class='nav'>\n"
           "        <div class='brand'><span class='logo' aria-hidden='true'></span><h1>Team Elevate Auctions</h1></div>\n"
           "        <nav class='links'>\n"
           "          <a href='/~elevate/cgi/index.cgi'>Home</a>\n"
           "          <a href='/~elevate/cgi/list_auctions.cgi'>Browse</a>\n"
           "          <a href='/~elevate/cgi/my_bids.cgi'>My Bids</a>\n"
           "        </nav>\n"
           "      </div>\n"
           "    </div>\n"
           "  </header>\n"
           "  <main class='center'>\n"
           "    <div class='card'>\n"
           "      <h2 class='title'>Login successful</h2>\n"
           "      <p class='muted'>Signed in as <strong>";
        cout << email;
        cout << "</strong>. Redirecting to the homepage…</p>\n"
                "      <a class='btn' href='/~elevate/cgi/index.cgi'>Go to Home</a>\n"
                "      <p class='small'>If you aren't redirected automatically, click the button above.</p>\n"
                "    </div>\n"
                "  </main>\n"
                "</body>\n"
                "</html>\n";
    } else {
        // ===== FAILURE (buttons for Try again / Create account) =====
        cout << "Content-Type: text/html\n\n";
        cout
        << "<!DOCTYPE html>\n"
           "<html lang='en'>\n"
           "<head>\n"
           "  <meta charset='utf-8'>\n"
           "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
           "  <title>Login Failed · Team Elevate</title>\n"
           "  <style>\n"
           "    :root{--ink:#0f172a;--muted:#475569;--bg:#f6f7fb;--card:#ffffff;--brand:#4f46e5}\n"
           "    *{box-sizing:border-box}\n"
           "    html,body{margin:0;padding:0}\n"
           "    body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;color:var(--ink);background:var(--bg);line-height:1.5}\n"
           "    header{background:linear-gradient(135deg,#111827,#1f2937 50%,#111827);color:#fff}\n"
           "    .wrap{max-width:1024px;margin:0 auto;padding:0 20px}\n"
           "    .nav{display:flex;align-items:center;justify-content:space-between;padding:14px 0}\n"
           "    .brand{display:flex;gap:10px;align-items:center}\n"
           "    .logo{width:32px;height:32px;border-radius:10px;background:linear-gradient(135deg,var(--brand),#7c3aed)}\n"
           "    .brand h1{font-size:17px;margin:0;font-weight:700;letter-spacing:.3px}\n"
           "    .links a{color:#e5e7eb;text-decoration:none;margin-left:12px;font-weight:600}\n"
           "    .links a:hover{color:#fff;text-decoration:underline}\n"
           "    main{padding:22px 0}\n"
           "    .center{display:grid;place-items:center}\n"
           "    .card{background:var(--card);border-radius:16px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:22px;width:100%;max-width:560px}\n"
           "    .title{margin:6px 0 8px;font-size:24px}\n"
           "    .error{color:#dc2626;margin:8px 0 16px}\n"
           "    .row{display:flex;gap:12px;flex-wrap:wrap}\n"
           "    .btn{flex:1 1 240px;display:block;padding:14px 16px;border-radius:14px;background:var(--brand);color:#fff;font-weight:800;text-decoration:none;text-align:center}\n"
           "    .btn:hover{filter:brightness(1.05)}\n"
           "  </style>\n"
           "</head>\n"
           "<body>\n"
           "  <header>\n"
           "    <div class='wrap'>\n"
           "      <div class='nav'>\n"
           "        <div class='brand'><span class='logo' aria-hidden='true'></span><h1>Team Elevate Auctions</h1></div>\n"
           "        <nav class='links'>\n"
           "          <a href='/~elevate/cgi/index.cgi'>Home</a>\n"
           "          <a href='/~elevate/cgi/register.cgi'>Register</a>\n"
           "        </nav>\n"
           "      </div>\n"
           "    </div>\n"
           "  </header>\n"
           "  <main class='center'>\n"
           "    <div class='card'>\n"
           "      <h2 class='title'>Login Failed</h2>\n"
           "      <p class='error'>Invalid email or password.</p>\n"
           "      <div class='row'>\n"
           "        <a class='btn' href='/~elevate/cgi/login.cgi'>Try again</a>\n"
           "        <a class='btn' href='/~elevate/cgi/register.cgi'>Create an account</a>\n"
           "      </div>\n"
           "    </div>\n"
           "  </main>\n"
           "</body>\n"
           "</html>\n";
    }

    mysql_free_result(result);
    mysql_close(conn);

    return 0;
}
