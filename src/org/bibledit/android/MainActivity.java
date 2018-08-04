package org.bibleconsultants.jezusredt;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebChromeClient;


public class MainActivity extends Activity
{
  WebView webview = null;
  
  // Function is called when the app gets launched.
  @Override
  public void onCreate (Bundle savedInstanceState)
  {
    super.onCreate (savedInstanceState);
    webview = new WebView (this);
    setContentView (webview);
    webview.getSettings().setJavaScriptEnabled (true);
    webview.getSettings().setBuiltInZoomControls (true);
    webview.getSettings().setSupportZoom (true);
    webview.setWebViewClient(new WebViewClient());
    // Set high quality client.
    webview.setWebChromeClient(new WebChromeClient() {
    });
    webview.loadUrl ("file:///android_asset/index.html");
  }

  @Override
  public boolean onCreateOptionsMenu (Menu menu)
  {
    return false;
  }

  @Override
  public void onBackPressed() {
    // The Android back button navigates back in the web view.
    // This is the behaviour people expect.
    if (webview.canGoBack()) {
      webview.goBack();
      return;
    }
    // Otherwise defer to system default behavior.
    super.onBackPressed();
  }
  
}
