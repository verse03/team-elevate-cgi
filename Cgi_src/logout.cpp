#include <iostream>
using namespace std;

int main() {
    // ===== HEADERS (no output before this) =====
    cout << "Content-Type: text/html\r\n";
    cout << "Set-Cookie: user_id=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Max-Age=0; HttpOnly\r\n";
    cout << "Set-Cookie: user_email=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Max-Age=0\r\n";
    cout << "Cache-Control: no-store, no-cache, must-revalidate\r\n";
    cout << "Pragma: no-cache\r\n";
    cout << "\r\n"; // end headers

    // BODY
    cout
    << "<!DOCTYPE html>\n"
       "<html lang='en'>\n"
       "<head>\n"
       "  <meta charset='utf-8'>\n"
       "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
       // ✅ FIXED: integer delay + space before url
       "  <meta http-equiv='refresh' content='2; url=/~elevate/cgi/index.cgi'>\n"
       "  <title>Logged Out · Team Elevate</title>\n"
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
       "    .card{background:var(--card);border-radius:16px;box-shadow:0 8px 22px rgba(2,6,23,.10);padding:24px;width:100%;max-width:520px;text-align:center}\n"
       "    .muted{color:var(--muted)}\n"
       "    a{color:var(--brand);text-decoration:none}\n"
       "    a:hover{text-decoration:underline}\n"
       "    .btn{display:inline-block;margin-top:12px;padding:10px 14px;border-radius:10px;background:var(--brand);color:#fff;font-weight:700}\n"
       "    .btn:hover{filter:brightness(1.05)}\n"
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
       "          <a href='/~elevate/cgi/login.cgi'>Login</a>\n"
       "          <a href='/~elevate/cgi/register.cgi'>Register</a>\n"
       "        </nav>\n"
       "      </div>\n"
       "    </div>\n"
       "  </header>\n"
       "  <main class='center'>\n"
       "    <div class='card'>\n"
       "      <h1>✓ Signed out</h1>\n"
       "      <p class='muted'>Your session has ended. We’re taking you back to the homepage…</p>\n"
       "      <a class='btn' href='/~elevate/cgi/index.cgi'>Go to Home</a>\n"
       "    </div>\n"
       "  </main>\n"
       "</body>\n"
       "</html>\n";

    return 0;
}
