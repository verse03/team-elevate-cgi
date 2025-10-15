#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

int main() {
    // REQUIRED CGI HEADER
    cout << "Content-type: text/html\n\n";

    // Check if user is logged in (read cookies)
    char* httpCookie = getenv("HTTP_COOKIE");
    string userEmail = "";
    bool isLoggedIn = false;

    if (httpCookie) {
        string cookies(httpCookie);
        size_t pos = cookies.find("user_email=");
        if (pos != string::npos) {
            size_t start = pos + 11; // length of "user_email="
            size_t end = cookies.find(";", start);
            if (end == string::npos) end = cookies.length();
            userEmail = cookies.substr(start, end - start);
            isLoggedIn = true;
        }
    }

    // HTML5 page output
    cout
    << "<!DOCTYPE html>\n"
       "<html lang='en'>\n"
       "<head>\n"
       "  <meta charset='UTF-8'>\n"
       "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
       "  <title>Auction Site - Team Elevate</title>\n"
       "  <style>\n"
       "    :root{--ink:#0f172a;--muted:#475569;--bg:#f6f7fb;--card:#ffffff;--brand:#4f46e5;--brand-2:#22c55e}\n"
       "    *{box-sizing:border-box}\n"
       "    html,body{margin:0;padding:0}\n"
       "    body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;color:var(--ink);background:var(--bg);line-height:1.5}\n"
       "    header{background:linear-gradient(135deg,#111827,#1f2937 50%,#111827);color:#fff}\n"
       "    .wrap{max-width:1024px;margin:0 auto;padding:0 20px}\n"
       "    .nav{display:flex;align-items:center;justify-content:space-between;padding:18px 0}\n"
       "    .brand{display:flex;gap:10px;align-items:center}\n"
       "    .logo{width:36px;height:36px;border-radius:10px;background:linear-gradient(135deg,var(--brand),#7c3aed);display:inline-block}\n"
       "    .brand h1{font-size:18px;margin:0;font-weight:700;letter-spacing:.3px}\n"
       "    .links a{color:#e5e7eb;text-decoration:none;margin-left:14px;font-weight:600}\n"
       "    .links a:hover{color:#fff;text-decoration:underline}\n"
       "    .hero{display:grid;grid-template-columns:1.2fr .8fr;gap:28px;align-items:center;padding:28px 0 42px}\n"
       "    .hero h2{margin:0 0 12px;font-size:34px;line-height:1.15}\n"
       "    .hero p{margin:0;color:#cbd5e1}\n"
       "    .cta{margin-top:18px;display:flex;gap:12px;flex-wrap:wrap}\n"
       "    .btn{display:inline-block;padding:10px 14px;border-radius:10px;text-decoration:none;font-weight:700}\n"
       "    .btn.primary{background:var(--brand);color:#fff}\n"
       "    .btn.ghost{background:rgba(255,255,255,.1);color:#fff;border:1px solid rgba(255,255,255,.25)}\n"
       "    main{padding:22px 0}\n"
       "    .card{background:var(--card);border-radius:14px;box-shadow:0 6px 18px rgba(2,6,23,.08);padding:18px}\n"
       "    .grid{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:16px;margin-top:16px}\n"
       "    .feature h3{margin:0 0 6px;font-size:18px}\n"
       "    .muted{color:var(--muted)}\n"
       "    .user-info{background:#e8f5e9;padding:10px;border-radius:10px;margin:14px 0}\n"
       "    footer{margin:28px 0 22px;color:var(--muted);font-size:14px}\n"
       "    @media (max-width:900px){.hero{grid-template-columns:1fr}.grid{grid-template-columns:1fr 1fr}}\n"
       "    @media (max-width:640px){.grid{grid-template-columns:1fr}}\n"
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
       "        <nav class='links'>\n";

    if (isLoggedIn) {
        cout
        << "          <a href='index.cgi'>Home</a>\n"
           "          <a href='list_auctions.cgi'>Browse Auctions</a>\n"
           "          <a href='my_bids.cgi'>My Bids</a>\n"
           "          <a href='logout.cgi'>Logout</a>\n";
    } else {
        cout
        << "          <a href='index.cgi'>Home</a>\n"
           "          <a href='login.cgi'>Login</a>\n"
           "          <a href='register.cgi'>Register</a>\n";
    }

    cout
    << "        </nav>\n"
       "      </div>\n"

       // HERO
       "      <section class='hero' aria-label='Welcome'>\n"
       "        <div>\n"
       "          <h2>Bid. Win. Elevate.</h2>\n"
       "          <p>Trusted listings, transparent bidding, and real-time results — all in one place.</p>\n"
       "          <div class='cta'>\n";

    if (!isLoggedIn) {
        cout
        << "            <a class='btn primary' href='/~elevate/cgi/register.cgi'>Create an account</a>\n"
           "            <a class='btn ghost' href='/~elevate/cgi/login.cgi'>Log in</a>\n";
    } else {
        cout
        << "            <a class='btn primary' href='/~elevate/cgi/list_auctions.cgi'>Browse auctions</a>\n"
           "            <a class='btn ghost' href='/~elevate/cgi/my_bids.cgi'>View my bids</a>\n";
    }

    cout
    << "          </div>\n"
       "        </div>\n"
       "        <div class='card' style='color:#374151;'>\n"
       "          <h3 style='margin-top:0;color:#374151;'>Today&apos;s Highlights</h3>\n"
       "          <ul style='margin:8px 0 0; padding-left:18px; color:#374151;'>\n"
       "            <li>Live updates as bids change</li>\n"
       "            <li>Verified sellers &amp; secure checkout</li>\n"
       "            <li>Watchlist to track your favorites</li>\n"
       "          </ul>\n"
       "        </div>\n"
       "      </section>\n"
       "    </div>\n"
       "  </header>\n"

       "  <main>\n"
       "    <div class='wrap'>\n";

    if (isLoggedIn) {
        cout
        << "      <div class='user-info'>\n"
           "        <p>✓ Logged in as: <strong>" << userEmail << "</strong></p>\n"
           "      </div>\n";
    }

    cout
    << "      <section class='card'>\n"
       "        <h2 style='margin-top:0'>Why choose Team Elevate?</h2>\n"
       "        <div class='grid'>\n"
       "          <div class='feature'>\n"
       "            <h3>Fair &amp; Transparent</h3>\n"
       "            <p class='muted'>Clear rules, visible bid history, and no hidden fees.</p>\n"
       "          </div>\n"
       "          <div class='feature'>\n"
       "            <h3>Fast &amp; Secure</h3>\n"
       "            <p class='muted'>Optimized checkout and encrypted sessions keep you safe.</p>\n"
       "          </div>\n"
       "          <div class='feature'>\n"
       "            <h3>Built for You</h3>\n"
       "            <p class='muted'>Personalized watchlists and alerts so you never miss a win.</p>\n"
       "          </div>\n"
       "        </div>\n"
       "      </section>\n"
       "      <footer>\n"
       "        &copy; 2025 Team Elevate. All rights reserved.\n"
       "      </footer>\n"
       "    </div>\n"
       "  </main>\n"
       "</body>\n"
       "</html>\n";

    return 0;
}