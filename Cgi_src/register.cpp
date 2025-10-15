#include <iostream>
#include <string>
#include <map>
#include <mysql/mysql.h>
#include <cstdlib>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>

using namespace std;

/* -------------------- Helpers -------------------- */

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

// Simple SHA-256 hash
string hashPassword(const string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Simple email validation
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos &&
            atPos > 0 && dotPos > atPos + 1 && dotPos < email.length() - 1);
}

/* -------------------- UI Shell -------------------- */

// Shared head + styles (keeps your purple + dark header theme)
void printHead(const string& title) {
    cout << "Content-Type: text/html\n\n";
    cout << "<!doctype html>\n<html lang='en'>\n<head>\n"
         << "  <meta charset='utf-8'>\n"
         << "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
         << "  <title>" << title << "</title>\n"
         << "  <style>\n"
         << "    :root{--bg:#0f172a;--bg2:#111827;--card:#ffffff;--muted:#6b7280;"
            " --text:#0f172a;--ink:#111827;--ink2:#374151;--soft:#f3f4f6;--accent:#5b3bf1;--accent-ink:#ffffff;}\n"
         << "    *{box-sizing:border-box} body{margin:0;font-family:Inter,system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;background:#f3f4f8;color:var(--ink)}\n"
         << "    .topbar{background:linear-gradient(180deg,var(--bg),var(--bg2));color:#fff;padding:18px 24px;display:flex;align-items:center;justify-content:space-between}\n"
         << "    .brand{display:flex;align-items:center;gap:12px;font-weight:800;font-size:20px}\n"
         << "    .dot{width:22px;height:22px;border-radius:8px;background:var(--accent)}\n"
         << "    nav a{color:#e5e7eb;text-decoration:none;margin-left:22px;font-weight:600}\n"
         << "    nav a:hover{color:#fff}\n"
         << "    .wrap{max-width:980px;margin:32px auto;padding:0 20px}\n"
         << "    .card{background:var(--card);border-radius:18px;box-shadow:0 10px 25px rgba(31,41,55,.08);padding:28px}\n"
         << "    h1{margin:0 0 10px;font-size:28px;color:var(--ink)}\n"
         << "    p.muted{color:var(--muted);margin:0 0 20px}\n"
         << "    form{display:flex;flex-direction:column;gap:14px;margin-top:10px}\n"
         << "    label{font-weight:700;color:var(--ink2);font-size:14px}\n"
         << "    input{width:100%;padding:12px 14px;border:1px solid #e5e7eb;border-radius:12px;font-size:15px;background:#fff}\n"
         << "    input:focus{outline:2px solid var(--accent);border-color:transparent}\n"
         << "    .btn{display:inline-block;background:var(--accent);color:var(--accent-ink);border:none;border-radius:12px;padding:12px 16px;font-weight:700;cursor:pointer}\n"
         << "    .btn:hover{filter:brightness(1.05)}\n"
         << "    .error{background:#fef2f2;border:1px solid #fecaca;color:#991b1b;padding:12px 14px;border-radius:12px;margin:8px 0}\n"
         << "    .success{background:#ecfdf5;border:1px solid #a7f3d0;color:#065f46;padding:12px 14px;border-radius:12px;margin:8px 0}\n"
         << "    .helper{color:var(--muted);font-size:13px}\n"
         << "    .footer{color:#6b7280;font-size:13px;margin:36px 0;text-align:center}\n"
         << "    .top-gap{margin-top:34px}\n"
         << "  </style>\n"
         << "</head>\n<body>\n"
         << "<header class='topbar'>\n"
         << "  <div class='brand'><div class='dot'></div><div>Team Elevate Auctions</div></div>\n"
         << "  <nav>\n"
         << "    <a href='/~elevate/cgi/index.cgi'>Home</a>\n"
         << "    <a href='/~elevate/cgi/login.cgi'>Login</a>\n"
         << "    <a href='/~elevate/cgi/register.cgi'>Register</a>\n"
         << "  </nav>\n"
         << "</header>\n"
         << "<main class='wrap'>\n";
}

void printTail() {
    cout << "  <div class='footer'>&copy; 2025 Team Elevate. All rights reserved.</div>\n"
         << "</main>\n</body>\n</html>\n";
}

void renderForm(const string& emailPrefill = "", const string& errorMsg = "") {
    printHead("Create your account");
    cout << "  <section class='card'>\n"
         << "    <h1>Create your account</h1>\n"
         << "    <p class='muted'>Join Team Elevate to start bidding and tracking your favorites.</p>\n";
    if (!errorMsg.empty()) {
        cout << "    <div class='error'>" << errorMsg << "</div>\n";
    }
    cout << "    <form method='post' action='/~elevate/cgi/register.cgi'>\n"
         << "      <div>\n"
         << "        <label for='email'>Email</label>\n"
         << "        <input id='email' name='email' type='email' maxlength='100' required value='" << emailPrefill << "'>\n"
         << "      </div>\n"
         << "      <div>\n"
         << "        <label for='password'>Password</label>\n"
         << "        <input id='password' name='password' type='password' minlength='8' required>\n"
         << "        <div class='helper'>Use at least 8 characters.</div>\n"
         << "      </div>\n"
         << "      <div>\n"
         << "        <label for='confirm'>Confirm Password</label>\n"
         << "        <input id='confirm' name='confirm' type='password' minlength='8' required>\n"
         << "      </div>\n"
         << "      <button class='btn' type='submit'>Create account</button>\n"
         << "    </form>\n"
         << "    <div class='top-gap helper'>Already have an account? <a href='/~elevate/cgi/login.cgi'>Log in</a>.</div>\n"
         << "  </section>\n";
    printTail();
}

void renderMessage(const string& title, const string& messageHtml, bool success=false) {
    printHead(title);
    cout << "  <section class='card'>\n"
         << "    <h1>" << title << "</h1>\n"
         << (success ? "    <div class='success'>" : "    <div class='error'>")
         << messageHtml << "</div>\n"
         << "    <div class='top-gap'><a class='btn' href='/~elevate/cgi/register.cgi'>Back to Register</a>\n"
         << "    &nbsp; <a class='btn' style='background:#1f2937' href='/~elevate/cgi/login.cgi'>Go to Login</a></div>\n"
         << "  </section>\n";
    printTail();
}

/* -------------------- Main -------------------- */

int main() {
    // GET -> show form
    const char* requestMethod = getenv("REQUEST_METHOD");
    if (!requestMethod || string(requestMethod) != "POST") {
        renderForm();
        return 0;
    }

    // Parse POST data
    map<string, string> postData = parsePostData();
    string email   = postData["email"];
    string password= postData["password"];
    string confirm = postData["confirm"];

    // Basic validation
    if (email.empty() || password.empty() || confirm.empty()) {
        renderForm(email, "All fields are required.");
        return 0;
    }
    if (!isValidEmail(email)) {
        renderForm(email, "Please enter a valid email address.");
        return 0;
    }
    if (password.length() < 8) {
        renderForm(email, "Password must be at least 8 characters.");
        return 0;
    }
    if (password != confirm) {
        renderForm(email, "Passwords do not match.");
        return 0;
    }

    // Connect to MariaDB
    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL) {
        renderMessage("Database Error", "Failed to initialize MySQL.");
        return 0;
    }

    // --- UPDATE if your credentials change ---
    const char* host = "localhost";
    const char* user = "cs370_section2_elevate";
    const char* pass = "etavele_004";
    const char* db   = "cs370_section2_elevate";

    if (mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0) == NULL) {
        string err = mysql_error(conn);
        mysql_close(conn);
        renderMessage("Database Connection Error", err);
        return 0;
    }

    // Check if email exists
    char escapedEmail[201];
    mysql_real_escape_string(conn, escapedEmail, email.c_str(), email.length());

    string checkQuery = "SELECT user_id FROM users WHERE user_email='";
    checkQuery += escapedEmail;
    checkQuery += "'";

    if (mysql_query(conn, checkQuery.c_str())) {
        string err = mysql_error(conn);
        mysql_close(conn);
        renderMessage("Query Error", err);
        return 0;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (row) {
        mysql_free_result(result);
        mysql_close(conn);
        renderForm(email, "This email is already registered. Try a different one or log in.");
        return 0;
    }
    mysql_free_result(result);

    // Insert user
    string hashedPassword = hashPassword(password);
    char escapedHash[513];
    mysql_real_escape_string(conn, escapedHash, hashedPassword.c_str(), hashedPassword.length());

    string insertQuery =
        "INSERT INTO users (user_email, password_hash, joindate) VALUES ('" +
        string(escapedEmail) + "', '" + string(escapedHash) + "', NOW())";

    if (mysql_query(conn, insertQuery.c_str())) {
        string err = mysql_error(conn);
        mysql_close(conn);
        renderMessage("Registration Error", err);
        return 0;
    }

    unsigned long long newUserId = mysql_insert_id(conn);
    mysql_close(conn);

    // Success: set cookies + auto-redirect
    cout << "Content-Type: text/html\n";
    cout << "Set-Cookie: user_id=" << newUserId << "; Path=/; HttpOnly; SameSite=Lax\n";
    cout << "Set-Cookie: user_email=" << email << "; Path=/; SameSite=Lax\n\n";

    cout << "<!doctype html><html lang='en'><head>"
         << "<meta charset='utf-8'>"
         << "<meta http-equiv='refresh' content='2;url=/~elevate/cgi/index.cgi'>"
         << "<title>Registration Successful</title></head><body>";

    // Re-use our pretty shell for the success state
    printHead("Registration Successful");
    cout << "  <section class='card'>\n"
         << "    <h1>✓ Registration Successful</h1>\n"
         << "    <div class='success'>Welcome to Team Elevate, " << email << ".</div>\n"
         << "    <p class='muted'>You’ll be redirected to the home page momentarily.</p>\n"
         << "    <div class='top-gap'><a class='btn' href='/~elevate/cgi/index.cgi'>Go now</a></div>\n"
         << "  </section>\n";
    printTail();

    return 0;
}
