ofxColourLoversHelper
------------------------------------

# Overview
An **openFrameworks** helper addon to handle **ColourLovers** palettes.
 
The core of this add-on is https://github.com/borg/ofxColourLovers.  
Thanks to **@borg**!  

I just added some methods to help integrate into my projects/addons.  
Switched from *ofxUI* to *ofxImGui* and better user control.  
Switched from *ofxHttpUtils* to *ofxHTTP* to fix *Colour Lover API* CloudFlare updates.  

<!-- ## Screenshot -->
<!-- ![image](/readme_images/Capture_ofxImGui.PNG?raw=true "image")   -->

## Screencast
![image](/readme_images/ofxColourLoversHelper.gif?raw=true "gif")

## Features
- Implemented GUI with **ImGui**.  
- _ofxUI_ and _ofxHttpUtils_ are now deprecated.
- Favorites, palette previews...etc all referenced to your local (ofApp) palette.

## Usage
- Look the example.

## Dependencies
Add to your project using **PROJECT GENERATOR**:    
* [ofxImGui](https://github.com/Daandelange/ofxImGui)
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxHTTP](https://github.com/bakercp/ofxHTTP)
  * ofxSSLManager
  * ofxIO
  * ofxMediaType
  * ofxNetworkUtils
* ofxPoco [OF]
* ofxXmlSettings [OF]

<details>
  <summary>TODO</summary>
  <p>

- Fix singleton/threading when API searching. [?]
- Add more API queries like search/save/load by a logged user. [?]
  </p>
</details>

## Tested System
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2021.*  

Thanks to **@borg**, **@arturoc**, **@MacFurax** and **@Daandelange** for the repositories.  
Thanks to **@Kabra** for *HTTP POST/GET API helping*.  

## License
*MIT License.*
