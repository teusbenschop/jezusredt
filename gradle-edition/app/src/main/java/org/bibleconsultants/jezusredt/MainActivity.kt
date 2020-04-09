package org.bibleconsultants.jezusredt

import android.os.Bundle
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {

    lateinit var webview: WebView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        // https://android.jlelse.eu/kickstarting-android-development-with-kotlin-goodbye-findviewbyid-6df19e02f378
        //webview = WebView (this);
        // https://stackoverflow.com/questions/47872078/how-to-load-an-url-inside-a-webview-using-android-kotlin
        webview = findViewById(R.id.webview)

        // Set high quality client.
        //webview.setWebChromeClient(new WebChromeClient() {});

        webview.webViewClient = object : WebViewClient() {
            override fun shouldOverrideUrlLoading(view: WebView?, url: String?): Boolean {
                view?.loadUrl(url)
                return true
            }
        }

        // Load contents.
        //webview.loadUrl ("file:///android_asset/index.html");
        webview.loadUrl ("https://bibledit.org");
    }
}
