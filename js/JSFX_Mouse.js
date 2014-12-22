/******************************************************************* 
* 
* File    : JSFX_Mouse.js © JavaScript-FX.com
* 
* Created : 2000/07/15 
* 
* Author  : Roy Whittle  (Roy@Whittle.com) www.Roy.Whittle.com 
* 
* Purpose : To create a cross browser "Mouse" object.
*		This library will allow scripts to query the current x,y
*		coordinates of the mouse.
* 
* History 
* Date         Version        Description 
* 2000-06-08	2.0		Converted for javascript-fx
* 2001-08-26	2.1		Corrected a bug where IE6 was not detected.
***********************************************************************/
if(!window.JSFX)
	JSFX=new Object();
if(!JSFX.Browser)
	JSFX.Browser = new Object();

JSFX.Browser.mouseX = 0;
JSFX.Browser.mouseY = 0;

if(navigator.appName.indexOf("Netscape") != -1)
{
	JSFX.Browser.captureMouseXY = function (evnt) 
	{
		JSFX.Browser.mouseX=evnt.pageX;
		JSFX.Browser.mouseY=evnt.pageY;
	}

	window.captureEvents(Event.MOUSEMOVE);
	window.onmousemove = JSFX.Browser.captureMouseXY;
}
else if(document.all)
{
	if(document.getElementById)
		JSFX.Browser.captureMouseXY = function ()
		{
			JSFX.Browser.mouseX = event.x + document.body.scrollLeft;
			JSFX.Browser.mouseY = event.y + document.body.scrollTop;
		}
	else
		JSFX.Browser.captureMouseXY = function ()
		{
			JSFX.Browser.mouseX = event.x;
			JSFX.Browser.mouseY = event.y;
		}
	document.onmousemove = JSFX.Browser.captureMouseXY;
} 
/*** End  ***/ 