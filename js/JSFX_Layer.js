/******************************************************************* 
* File    : JSFX_Layer.js  © JavaScript-FX.com
* Created : 2001/04/11 
* Author  : Roy Whittle  (Roy@Whittle.com) www.Roy.Whittle.com 
* Purpose : To create a cross browser dynamic layers.
* History 
* Date         Version        Description 
* 2001-03-17	3.0		Completely re-witten for use by javascript-fx
* 2001-09-08	3.1		Added the ability for child layers
* 2001-09-23	3.2		Save a reference so we can use a self referencing 
*					timer (may not be required)
* 2001-09-28	3.3		Add a width for Netscape 4.x
* 2001-09-28	3.4		Remove width for Netscape 4.x (Not needed)
***********************************************************************/ 
var ns4 = (navigator.appName.indexOf("Netscape") != -1 && !document.getElementById);

if(!window.JSFX)
	JSFX=new Object();

JSFX.layerNo=0; 
/**********************************************************************************/
JSFX.createLayer = function(htmlStr, parent)
{
	var count=0;
	var elem = null;

 	if(document.layers) 
	{
		var xName="xLayer" + JSFX.layerNo++;
		if(parent == null)
			elem=new Layer(2000);
		else
			elem=new Layer(2000, parent.elem);
 
		elem.document.open(); 
		elem.document.write(htmlStr); 
		elem.document.close(); 
		elem.moveTo(0,0);
		elem.innerHTML = htmlStr;
	}
	else 
	if(document.all) 
	{
		if(parent == null)
			parent=document.body;
		else
			parent=parent.elem;

		var xName = "xLayer" + JSFX.layerNo++; 
		var txt = '<DIV ID="' + xName + '"'
			+ ' STYLE="position:absolute;left:0;top:0;visibility:hidden;font-size:0.24em;">' 
			+ htmlStr 
			+ '</DIV>'; 

			parent.insertAdjacentHTML("BeforeEnd",txt); 

		elem = document.all[xName]; 
	} 
	else 
	if (document.getElementById)
	{
		if(parent == null)
			parent=document.body;
		else
			parent=parent.elem;

		var xName="xLayer" + JSFX.layerNo++;
		var txt = ""
			+ "position:absolute;left:0px;top:0px;visibility:hidden;font-size:0.24em;";

		var newRange = document.createRange();

		elem = document.createElement("DIV");
		elem.setAttribute("style",txt);
		elem.setAttribute("id", xName);

		parent.appendChild(elem);

		newRange.setStartBefore(elem);
		strFrag = newRange.createContextualFragment(htmlStr);	
		elem.appendChild(strFrag);
	}

	return elem;
}
/**********************************************************************************/
JSFX.Layer = function(newLayer, parent) 
{
	if(!newLayer)
		return;

	if(typeof newLayer == "string")
		this.elem = JSFX.createLayer(newLayer, parent);
	else
		this.elem=newLayer;

	if(document.layers)
	{
		this.images		= this.elem.document.images; 
		this.parent		= parent;
		this.style		= this.elem;
		if(parent != null)
			this.style.visibility = "inherit";
 	} 
	else 
	{
		this.images  = document.images; 
		this.parent	 = parent;
		this.style   = this.elem.style; 
	} 
	window[this.elem.id]=this;	//save a reference to this
} 
/**********************************************************************************/
JSFX.findLayer = function(theDiv, d)
{
	if(document.layers)
	{
		var i;
		if(d==null) d = document;
		var theLayer = d.layers[theDiv];
		if(theLayer != null)
			return(theLayer);
		else
			for(i=0 ; i<d.layers.length ; i++)
			{
				theLayer = JSFX.findLayer(theDiv, d.layers[i].document);
				if(theLayer != null)
					return(theLayer);
			}
		return("Undefined....");
	}
	else 
	if(document.all)
		return(document.all[theDiv]);
	else 
	if(document.getElementById)
		return(document.getElementById(theDiv));
	else
		return("Undefined.....");
}

/**********************************************************************************/
/*** moveTo (x,y) ***/
JSFX.Layer.prototype.moveTo = function(x,y)
{
	this.style.left = x+"px";
	this.style.top = y+"px";
}
if(ns4)
	JSFX.Layer.prototype.moveTo = function(x,y) { this.elem.moveTo(x,y); }
/**********************************************************************************/
/*** show()/hide() Visibility ***/
JSFX.Layer.prototype.show		= function() 	{ this.style.visibility = "visible"; } 
JSFX.Layer.prototype.hide		= function() 	{ this.style.visibility = "hidden"; } 
JSFX.Layer.prototype.isVisible	= function()	{ return this.style.visibility == "visible"; } 
if(ns4)
{
	JSFX.Layer.prototype.show		= function() 	{ this.style.visibility = "show"; }
	JSFX.Layer.prototype.hide 		= function() 	{ this.style.visibility = "hide"; }
	JSFX.Layer.prototype.isVisible 	= function() 	{ return this.style.visibility == "show"; }
}
/**********************************************************************************/
/*** zIndex ***/
JSFX.Layer.prototype.setzIndex	= function(z)	{ this.style.zIndex = z; } 
JSFX.Layer.prototype.getzIndex	= function()	{ return this.style.zIndex; }
/**********************************************************************************/
/*** ForeGround (text) Color ***/
JSFX.Layer.prototype.setColor	= function(c){this.style.color=c;}
if(ns4)
	JSFX.Layer.prototype.setColor	= function(c)
	{
		this.elem.document.write("<FONT COLOR='"+c+"'>"+this.elem.innerHTML+"</FONT>");
		this.elem.document.close();
	}
/**********************************************************************************/
/*** BackGround Color ***/
JSFX.Layer.prototype.setBgColor	= function(color) { this.style.backgroundColor = color==null?'transparent':color; } 
if(ns4)
	JSFX.Layer.prototype.setBgColor 	= function(color) { this.elem.bgColor = color; }
/**********************************************************************************/
/*** BackGround Image ***/
JSFX.Layer.prototype.setBgImage	= function(image) { this.style.backgroundImage = "url("+image+")"; }
if(ns4)
	JSFX.Layer.prototype.setBgImage 	= function(image) { this.style.background.src = image; }
/**********************************************************************************/
/*** set Content***/
JSFX.Layer.prototype.setContent   = function(xHtml)	{ this.elem.innerHTML=xHtml; } 
if(ns4)
	JSFX.Layer.prototype.setContent   = function(xHtml)
	{
		this.elem.document.write(xHtml);
		this.elem.document.close();
		this.elem.innerHTML = xHtml;
	}

/**********************************************************************************/
/*** Clipping ***/
JSFX.Layer.prototype.clip = function(x1,y1, x2,y2){ this.style.clip="rect("+y1+" "+x2+" "+y2+" "+x1+")"; }
if(ns4)
	JSFX.Layer.prototype.clip = function(x1,y1, x2,y2)
	{
		this.style.clip.top	=y1;
		this.style.clip.left	=x1;
		this.style.clip.bottom	=y2;
		this.style.clip.right	=x2;
	}
/**********************************************************************************/
/*** Resize ***/
JSFX.Layer.prototype.resizeTo = function(w,h)
{ 
	this.style.width	=w + "px";
	this.style.height	=h + "px";
}
if(ns4)
	JSFX.Layer.prototype.resizeTo = function(w,h)
	{
		this.style.clip.width	=w;
		this.style.clip.height	=h;
	}
/**********************************************************************************/
/*** getX/Y ***/
JSFX.Layer.prototype.getX	= function() 	{ return parseInt(this.style.left); }
JSFX.Layer.prototype.getY	= function() 	{ return parseInt(this.style.top); }
if(ns4)
{
	JSFX.Layer.prototype.getX	= function() 	{ return this.style.left; }
	JSFX.Layer.prototype.getY	= function() 	{ return this.style.top; }
}
/**********************************************************************************/
/*** getWidth/Height ***/
JSFX.Layer.prototype.getWidth		= function() 	{ return this.elem.offsetWidth; }
JSFX.Layer.prototype.getHeight	= function() 	{ return this.elem.offsetHeight; }
if(!document.getElementById)
	JSFX.Layer.prototype.getWidth		= function()
 	{ 
		//Extra processing here for clip
		return this.elem.scrollWidth;
	}

if(ns4)
{
	JSFX.Layer.prototype.getWidth		= function() 	{ return this.style.clip.right; }
	JSFX.Layer.prototype.getHeight	= function() 	{ return this.style.clip.bottom; }
}
/**********************************************************************************/
/*** Opacity ***/
if(ns4)
{
	JSFX.Layer.prototype.setOpacity = function(pc) {return 0;}
}
else if(document.all)
{
	JSFX.Layer.prototype.setOpacity = function(pc)
	{
		if(this.style.filter=="")
			this.style.filter="alpha(opacity=100);";
		this.elem.filters.alpha.opacity=pc;
	}
}
else
{
/*** Assume NS6 ***/
	JSFX.Layer.prototype.setOpacity = function(pc){	this.style.MozOpacity=pc+'%' }
}
/**************************************************************************/
/*** Event Handling - Start ***/
/*** NS4 ***/
if(ns4)
{
	JSFX.eventmasks = {
	      onabort:Event.ABORT, onblur:Event.BLUR, onchange:Event.CHANGE,
	      onclick:Event.CLICK, ondblclick:Event.DBLCLICK, 
	      ondragdrop:Event.DRAGDROP, onerror:Event.ERROR, 
	      onfocus:Event.FOCUS, onkeydown:Event.KEYDOWN,
	      onkeypress:Event.KEYPRESS, onkeyup:Event.KEYUP, onload:Event.LOAD,
	      onmousedown:Event.MOUSEDOWN, onmousemove:Event.MOUSEMOVE, 
	      onmouseout:Event.MOUSEOUT, onmouseover:Event.MOUSEOVER, 
	      onmouseup:Event.MOUSEUP, onmove:Event.MOVE, onreset:Event.RESET,
	      onresize:Event.RESIZE, onselect:Event.SELECT, onsubmit:Event.SUBMIT,
	      onunload:Event.UNLOAD
	};
	JSFX.Layer.prototype.addEventHandler = function(eventname, handler) 
	{
          this.elem.captureEvents(JSFX.eventmasks[eventname]);
          var xl = this;
      	this.elem[eventname] = function(event) { 
		event.clientX	= event.pageX;
		event.clientY	= event.pageY;
		event.button	= event.which;
		event.keyCode	= event.which;
		event.altKey	=((event.modifiers & Event.ALT_MASK) != 0);
		event.ctrlKey	=((event.modifiers & Event.CONTROL_MASK) != 0);
		event.shiftKey	=((event.modifiers & Event.SHIFT_MASK) != 0);
            return handler(xl, event);
        }
	}
	JSFX.Layer.prototype.removeEventHandler = function(eventName) 
	{
		this.elem.releaseEvents(JSFX.eventmasks[eventName]);
		delete this.elem[eventName];
	}
}
/**************************************************************************/
/** IE 4/5+***/
else
if(document.all)
{
	JSFX.Layer.prototype.addEventHandler = function(eventName, handler) 
	{
		var xl = this;
		this.elem[eventName] = function() 
		{ 
	            var e = window.event;
	            e.cancelBubble = true;
			if(document.getElementById)
			{
				e.layerX = e.offsetX;
				e.layerY = e.offsetY;
			}
			else
			{
				/*** Work around for IE 4 : clone window.event ***/
				ev = new Object();
				for(i in e)
					ev[i] = e[i];
				ev.layerX	= e.offsetX;
				ev.layerY	= e.offsetY;
				e = ev;
			}

	            return handler(xl, e); 
		}
	}
	JSFX.Layer.prototype.removeEventHandler = function(eventName) 
	{
		this.elem[eventName] = null;
	}
}
/**************************************************************************/
/*** Assume NS6 ***/
else
{
	JSFX.Layer.prototype.addEventHandler = function(eventName, handler) 
	{
		var xl = this;
		this.elem[eventName] = function(e) 
		{ 
	            e.cancelBubble = true;
	            return handler(xl, e);
		}
	}
	JSFX.Layer.prototype.removeEventHandler = function(eventName) 
	{
		this.elem[eventName] = null;
	}
}
/*** Event Handling - End ***/
/**************************************************************************/
JSFX.Layer.prototype.setTimeout = function(f, t) 
{
	setTimeout("window."+this.elem.id+"."+f, t);
}
