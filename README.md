ofxColourLoversHelper
------------------------------------

# Overview
**ofxColourLoversHelper** is an **openFrameworks** helper addon to handle **ColourLovers** palettes.
 
The core of this add-on is https://github.com/borg/ofxColourLovers.  
Thanks to **@borg**!  
I just added some methods to help integrate into my projects/addons.  
Switched from *ofxUI* to *ofxImGui* to better user control.  
Switched from *ofxHttpUtils* to *ofxHTTP* to fix Colour Lover API changes.  

<!-- ## Screenshot -->
<!-- ![image](/readme_images/Capture_ofxImGui.PNG?raw=true "image")   -->

## Screencast
![image](/readme_images/ofxColourLoversHelper.gif?raw=true "gif")

## Features
- Implemented GUI with **ImGui**.  
- _ofxUI_ and _ofxHttpUtils_ are now deprecated.

## Usage
- Look the example.

## Dependencies
Add to your project using *PROJECT GENERATOR*:    
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

## TODO
- Fix singleton/threading when API searching...

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2021.*  

Thanks to **@borg**, **@arturoc**, **@MacFurax** and **@Daandelange** for the repositories.  
Thanks to **@Kabra** for HTTP POST/GET helping.  

## License
*MIT License.*
