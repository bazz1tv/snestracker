/*******************************************************************
*
* File    : JSFX_Fire.js © JavaScript-FX.com
*
* Created : 2001/10/26
*
* Author  : Roy Whittle www.Roy.Whittle.com
*
* Purpose : To create a fire like mouse trailer. Can be customized to
*		look like a "Comet", "Rocket", "Sparkler" or "Flaming Torch"
*
* Requires	: JSFX_Layer.js - for layer creation, movement
*		: JSFX_Mouse.js - to track the mouse x,y coordinates
*
* History
* Date         Version        Description
*
* 2001-10-26	1.0		Created for javascript-fx
***********************************************************************/

var hexDigit=new Array("0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F");
var count=0;
var y=0;
function dec2hex(dec)
{
	return(hexDigit[dec>>4]+hexDigit[dec&15]);
}
function hex2dec(hex)
{
	return(parseInt(hex,16))
}

/*** Class FireSpark extends Layer ***/
JSFX.FireSpark = function(resetType)
{
	//Call the superclass constructor
	this.superC	= JSFX.Layer;
	this.superC("&nbsp;&nbsp;&nbsp;&nbsp;");


	this.count=0;

	this.dx 	= Math.random() * 4 - 2;
	this.dy	= Math.random() * 4 - 2;
	this.ay	= .05;
	this.x	= 100;
	this.y	= 100;
	if(resetType == 0)
		this.resetSpark = this.reset0;
	else if(resetType == 1)
		this.resetSpark = this.reset1;
	else if(resetType == 2)
		this.resetSpark = this.reset2;
	else if(resetType == 3)
		this.resetSpark = this.reset3;
	else
		this.resetSpark = this.reset0;
}
JSFX.FireSpark.prototype = new JSFX.Layer;
/*** END Class FireSpark Constructor - start methods ***/
JSFX.FireSpark.prototype.changeColour = function()
{
	var colour="";

	r2= Math.random()*255;
	g2= Math.random()*255;
	b2= Math.random()*255;

	if(!(r2 | g2 | b2))
	{
		r2=255;
		g2=255;
		b2=0;
	}

	colour = "#" + dec2hex(r2) + dec2hex(g2) + dec2hex(b2);
	this.setBgColor(colour);
}
JSFX.FireSpark.prototype.reset0 = function()
{

	
	
	if(Math.random() > .94)
	{
		if ( (this.omx == JSFX.Browser.mouseX && this.omy == JSFX.Browser.mouseY) )
		{
			
			if (this.count > 3)
			{
				this.hide();
				
				//this.hidden = true
			}
		}
		else
		{
			this.count = 0; 
			this.show();
		}
	
		
		this.x=JSFX.Browser.mouseX+6;
		this.y=JSFX.Browser.mouseY+12;
		
		this.dx = Math.random() * 1.5 + 0.5;
		this.dy = Math.random() * 2 + 2;
		this.changeColour();
		this.count++;
	}
	this.omx = JSFX.Browser.mouseX;
	this.omy = JSFX.Browser.mouseY;
	
}

JSFX.FireSpark.prototype.reset1 = function()
{
	if(Math.random() >.90)
	{
		this.x=JSFX.Browser.mouseX+4;
		this.y=JSFX.Browser.mouseY;
		this.dx = Math.random() * 6 + 2;
		this.dy = Math.random() * 2 - 1;
		this.changeColour();
	}
}
JSFX.FireSpark.prototype.reset2 = function()
{
	if(Math.random() >.80)
	{
		this.x=JSFX.Browser.mouseX - 5;
		this.y=JSFX.Browser.mouseY - 5;
		this.dx = Math.random() * 4 - 2;
		this.dy = Math.random() * 4 - 2;
		this.changeColour();
	}
}
JSFX.FireSpark.prototype.reset3 = function()
{
	if(Math.random() >.70)
	{
		this.x=JSFX.Browser.mouseX -1 + Math.random()*2;
		this.y=JSFX.Browser.mouseY-2;
		this.dx = Math.random() * 1 - 0.5;
		this.dy = Math.random() * -6 - 1;
		this.changeColour();
	}
}
JSFX.FireSpark.prototype.animate = function()
{
	//alert("DERP2");
	this.resetSpark();
	//this.show();
	

	this.dy += this.ay;
	this.x += this.dx;
	this.y += this.dy;
	this.moveTo(this.x, this.y);
	this.count++;
}
/*** END Class FireSpark Methods***/

/*** Class FireObj extends Object ***/
JSFX.FireObj = function(numStars, anim)
{
	this.id = "JSFX_FireObj_"+JSFX.FireObj.count++;
	this.sparks = new Array();
	for(i=0 ; i<numStars; i++)
	{
		this.sparks[i]=new JSFX.FireSpark(anim);
		this.sparks[i].clip(0,0,2,2);
		this.sparks[i].setBgColor("yellow");
		this.sparks[i].show();
	}
	window[this.id]=this;
	this.animate();
}
JSFX.FireObj.count = 0;
JSFX.FireObj.prototype.animate = function()
{
	setTimeout("window."+this.id+".animate()", 40);

	for(i=0 ; i<this.sparks.length ; i++)
		this.sparks[i].animate();

	

}
/*** END Class FireObj***/

/*** Create a static method for creating fire objects ***/
JSFX.Fire = function(numStars, anim)
{
	return new JSFX.FireObj(numStars, anim);
}

/*** If no other script has added it yet, add the ns resize fix ***/
if(navigator.appName.indexOf("Netscape") != -1 && !document.getElementById)
{
	if(!JSFX.ns_resize)
	{
		JSFX.ow = outerWidth;
		JSFX.oh = outerHeight;
		JSFX.ns_resize = function()
		{
			if(outerWidth != JSFX.ow || outerHeight != JSFX.oh )
				location.reload();
		}
	}
	window.onresize=JSFX.ns_resize;
}


