package org.bibleconsultants.jezusredt

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {

    lateinit var webview: WebView

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        // https://stackoverflow.com/questions/47872078/how-to-load-an-url-inside-a-webview-using-android-kotlin

        webview = findViewById(R.id.webview)

        // Set high quality client.
        //webview.setWebChromeClient(new WebChromeClient() {});

        webview.webViewClient = object : WebViewClient() {
            override fun shouldOverrideUrlLoading(view: WebView?, url: String?): Boolean {
                val externalUrl = url?.indexOf("http") === 0
                return if (externalUrl) {
                    val intent = Intent(Intent.ACTION_VIEW, Uri.parse(url))
                    view!!.context.startActivity(intent)
                    true
                } else {
                    false
                }
            }
        }

        // Load contents.
        webview.loadUrl ("file:///android_asset/index.html");
    }

    override fun onBackPressed() {
        // The Android back button navigates back in the web view.
        // This is the behaviour people expect.
        if (webview.canGoBack()) {
            webview.goBack()
            return
        }
        // Otherwise defer to system default behavior.
        super.onBackPressed()
    }

}
