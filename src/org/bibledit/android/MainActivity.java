package org.bibleconsultants.jezusredt;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebChromeClient;
import android.content.Intent;
import android.net.Uri;


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
    webview.setWebViewClient(new WebViewClient() {
      @Override
      public boolean shouldOverrideUrlLoading(WebView view, String url) {
        boolean externalUrl = (url.indexOf ("http") == 0);
        if (externalUrl) {
          Intent intent = new Intent (Intent.ACTION_VIEW, Uri.parse (url));
          view.getContext().startActivity(intent);
          return true;
        } else {
          return false;
        }
      }
    });
    // Set high quality client.
    //webview.setWebChromeClient(new WebChromeClient() {});
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

  /*
   The appearance of the app can be improved
   for example through the newer DrawerLayout.
   This was tried in 2019.
   When going to publish the app to the Google Play Console,
   the console said that the app will not be installable
   on about 20% of the devices.
   So if there's going to be evangelists with older telephones,
   they would not be able to use this app.
   For that reason the new DrawerLayout will not be used just now.
   Perhaps later.
   */
  

}
