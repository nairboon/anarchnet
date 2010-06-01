/*
UvumiTools Dock Menu v1.0.0 http://uvumi.com/tools/dock.html

Copyright (c) 2008 Uvumi LLC

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

var UvumiDock = new Class({

	Implements:Options,
	
	options:{
		position:'bottom',
		captionClassName:'dock-caption',
        offset: -32,
		minHeight:64,
		maxHeight:128,
		openDuration:'normal',
		hiddenOpacity:0.5,
		IEfixer:"css/blank.gif"
	},

	initialize:function(container,options,domcontainer){
		this.el = container;
        this.domcontainer = domcontainer;
		this.setOptions(options);
		this.margin=(this.options.minHeight/2).toInt();
		window.addEvents({
			'domready':this.domReady.bind(this),
			'resize':this.updateSize.bind(this)
		});
	},
	
	domReady:function(){
		this.el = $(this.el);
		
        if(this.domcontainer === null)
            this.domcontainer = document.body;
        else
            this.domcontainer = $(this.domcontainer);
            
		//check if we're dealing with a ul element
		if(this.el.get('tag')!='ul'){
			window.removeEvents('resize');
			return false;
		}
		
		//change ul display from block to inline
		this.el.setStyles({
			display:'inline',
			textAlign:'center'
		});
		
		//change li display from list to inline
		this.lis = $$(this.el.getElements('li'));
		this.lis.setStyles({
			display:'inline',
			listStyle:'none'
		});
		
		this.images = $$(this.el.getElements('img'));
		
		this.images.setStyles({
			border:0,
			height:this.options.minHeight
		});		
		
		//generate caption for each image from its alt property
		this.captions = [];
		this.images.each(function(image){
			var caption = new Element('span',{
				html:image.getProperty('alt'),
				'class':this.options.captionClassName,
				styles:{
					position:'absolute',
					top:-10,
					left:0,
					opacity:0
				},
				tween:{
					link:'cancel'
				}
			}).inject(image.getParent('li'));
			image.store('caption',caption);
			this.captions.push(caption);
		},this);
		
		this.images.addEvents({
			mouseenter:this.showCaption,
			mouseleave:this.hideCaption
		});

		//the dock will contain the ul
		this.dock = new Element('div',{
			styles:{
				textAlign:'center',
				opacity:this.options.hiddenOpacity
			},
			events:{
				mousemove:this.fishEye.bind(this),
				mouseenter:this.open.bind(this),
				mouseleave:this.close.bind(this)
			}
		});
		this.animFx = new Fx.Elements($$(this.dock,this.images),{
			duration:this.options.openDuration,
			link:'cancel'
		});

		
        this.dock.inject(this.domcontainer).setStyle('position','fixed').adopt(this.el);
		
		this.size = this.dock.getSize();
		this.updatePosition();
	},
	
	//open the dock when hovered.
	open:function(){
		var anim = {0:{
			opacity:1
		}};
		anim[0][this.animationParam]=0;
		this.animFx.start(anim);
	},
	
	//close the dock on mouseleave
	close:function(){
		var anim = {0:{
			opacity:this.options.hiddenOpacity
		}};
		anim[0][this.animationParam]=-this.size.y+this.margin+this.options.offset;
		for(var i=1;i<=this.images.length;i++){
			anim[i]={height:this.options.minHeight};
		}
		this.animFx.start(anim);
	},
	
	//you can dynamically change the dock position. Just set myDock.options,position to top or bottom, and call this function
	updatePosition:function(){
		switch(this.options.position){
			case 'top':
				var coords={
					top:-this.size.y+this.margin+this.options.offset,
					left:0,
					width:'100%'
				};
				this.animationParam = 'top';
				if(!Browser.Engine.gecko){
					this.lis.setStyle('verticalAlign','top');
				}
				$$(this.captions).setStyles({
					top:"",
					bottom:-10
				});
			break;
			case 'bottom':
				var coords={
					bottom:-this.size.y+this.margin,
					left:0,
					width:'100%'
				};
				this.animationParam = 'bottom';
				$$(this.captions).setStyles({
					top:-10,
					bottom:""
				});
			break;
		}
		this.dock.setStyles(coords);
	},
	
	//function called when window is resized
	updateSize:function(){
		this.size = this.dock.getSize();
		this.el.fireEvent('mouseleave');
	},
	
	//the most important function, called on mousemove over the dock.
	fishEye:function(e){
		this.images.each(function(image){
			cos = (image.getPosition().x+image.getSize().x/2 - e.client.x)/this.options.maxHeight;
			cos = Math.max(Math.min(cos,Math.PI/2),-Math.PI/2);
			image.setStyle('height',(Math.max((this.options.maxHeight*Math.cos(cos)),this.options.minHeight)).toInt());
			image.retrieve('caption').setStyle('left',e.client.x-15);
		},this);
		
	},
	
	//show caption over an image. 'this' do not refer to the dock object but to the image being hovered
	showCaption:function(){
		this.retrieve('caption').get('tween').start('opacity',1);
	},
	
	//hide caption when leaving an image. 'this' do not refer to the dock object but to the image left
	hideCaption:function(){
		this.retrieve('caption').get('tween').start('opacity',0);
	}
});

//temporary fix for opera until the mootools is fixed
Element.implement({
	getPosition: function(relative){
		if (isBody(this)) return {x: 0, y: 0};
		var offset = this.getOffsets(), scroll = this.getScrolls();
		var position = {x: offset.x - scroll.x, y: offset.y - scroll.y};
		var relativePosition = (relative && (relative = $(relative))) ? relative.getPosition() : {x: 0, y: 0};
        if (Browser.Engine.presto925) {
            var position = {x: offset.x, y: offset.y};
        }
		return {x: position.x - relativePosition.x, y: position.y - relativePosition.y};
	}
});

function isBody(element){
	return (/^(?:body|html)$/i).test(element.tagName);
};