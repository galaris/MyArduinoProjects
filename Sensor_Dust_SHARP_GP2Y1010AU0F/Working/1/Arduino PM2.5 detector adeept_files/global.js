/*
Powered by ueeshop.com		http://www.ueeshop.com
广州联雅网络科技有限公司		020-83226791
*/

var global_obj={	
	check_form:function(notnull_obj, format_obj, type){
		var flag=false;
		if(notnull_obj){
			notnull_obj.each(function(){
				if($.trim($(this).val())==''){
					$(this).css('border', '1px solid red');
					flag==false && ($(this).focus());
					flag=true;
				}else{
					$(this).removeAttr('style');
				}
			});
			if(flag){return flag;};
		}
		if(format_obj){
			var reg={
				'MobilePhone':/^1[34578]\d{9}$/,
				'Telephone':/^(0\d{2,3})(-)?(\d{7,8})(-)?(\d{3,})?$/,
				'Fax':/^(0\d{2,3})(-)?(\d{7,8})(-)?(\d{3,})?$/,
				'Email':/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/,
				'Length':/^.*/
			};
			var tips={
				'MobilePhone':lang_obj.format.mobilephone,
				'Telephone':lang_obj.format.telephone,
				'Fax':lang_obj.format.fax,
				'Email':lang_obj.format.email,
				'Length':lang_obj.format.length
			};
			if(type==1){
				format_obj.each(function(){
					var o=$(this);
					if($.trim(o.val())){
						if(reg[o.attr('format')].test($.trim(o.val()))===false){
							o.css('border', '1px solid red');
							o.focus();
							flag=true;
						}
					}
				});
			}else{
				format_obj.each(function(){
					var o=$(this);
					var s=o.attr('format').split('|');
					if(!$.trim(o.val())){
						flag=false;
					}else if((s[0]=='Length' && $.trim(o.val()).length!=parseInt(s[1])) || (s[0]!='Length' && reg[s[0]].test($.trim(o.val()))===false)){
						global_obj.win_alert(tips[s[0]].replace('%num%', s[1]));
						o.css('border', '1px solid red');
						o.focus();
						flag=true;
						return false;
					}
				});
			}
		}
		return flag;
	},
	
	win_alert:function(tips, callback, type){
		var type=(typeof(arguments[2])=='undefined')?'alert':arguments[2], html;
		$('#div_mask, .win_alert').remove();//优先清空多余的弹出框
		global_obj.div_mask();
		html='<div class="win_alert">';
			html+='<div class="win_close"><button class="close">x</button></div>';
			html+='<div class="win_tips">'+tips+'</div>';
			html+='<div class="win_btns"><button class="btn btn_sure">'+lang_obj.global.confirm+'</button>';
			if(type=='confirm') html+='<button class="btn btn_cancel">'+lang_obj.global.cancel+'</button>';
			html+='</div>';
		html+='</div>';
		$('body').prepend(html);
		$('.win_alert').css({left:$(window).width()/2-200});
		if(type=='confirm'){
			$('.win_alert').delegate('.close, .btn_cancel', 'click', function(){
				$('.win_alert').remove();
				global_obj.div_mask(1);
			}).delegate('.btn_sure', 'click', function(){
				$.isFunction(callback) && callback();
				$('.win_alert .close').click();
			});
			/*$(document).keyup(function(event){	//Esc、Space取消提示，空格、Enter确定提示
				if(event.keyCode==27 || event.keyCode==8){
					$('.win_alert .close').click();
				}else if(event.keyCode==32 || event.keyCode==13){
					$.isFunction(callback) && callback();
					$('.win_alert .close').click();
				}
			});*/
		}else{
			$('.win_alert').delegate('.close, .btn_sure', 'click', function(){
				$.isFunction(callback) && callback();
				$('.win_alert').remove();
				global_obj.div_mask(1);
			});
			/*$(document).keyup(function(event){	//Esc、Enter、Space、空格取消提示
				if(event.keyCode==13 || event.keyCode==8 || event.keyCode==27 || event.keyCode==32) {
					$('.win_alert .close').click();
				}
			});*/
		}
		return false;
	},
	
	div_mask:function(remove){
		if(remove==1){
			$('#div_mask').remove();
		}else{
			if(!$('#div_mask').size()){
				$('body').prepend('<div id="div_mask"></div>');
				$('#div_mask').css({width:'100%', height:$(document).height(), overflow:'hidden', position:'fixed', top:0, left:0, background:'#000', opacity:0.6, 'z-index':10000});
			}
		}
	},
	
	data_posting:function(display, tips){
		if(display){
			$('body').prepend('<div id="data_posting"><img src="/static/ico/data_posting.gif" width="16" height="16" align="absmiddle" />&nbsp;&nbsp;'+tips+'</div>');
			$('#data_posting').css({
				width:'188px',
				height:'24px',
				'line-height':'24px',
				padding:'0 8px',
				overflow:'hidden',
				border:'1px solid #bbb',
				background:'#ddd',
				position:'fixed',
				top:'40%',
				left:'48%',
				'z-index':10001
			});
		}else{
			setTimeout('$("#data_posting").remove();', 500);
		}
	},
	
	urlencode:function(str) {  
		str = (str + '').toString();   
		return encodeURIComponent(str).replace(/!/g, '%21').replace(/'/g, '%27').replace(/\(/g, '%28').replace(/\)/g, '%29').replace(/\*/g, '%2A').replace(/%20/g, '+');  
	},
	
	json_encode_data:function(value){
		var result=new Object();
		if(typeof(value)=='string' && value){
			if(typeof(JSON)=='object'){
				result=JSON.parse(value);
			}else{
				var ary, arr, key;
				ary=value.substr(1,value.length-2).split(',"');//踢走{}
				for(var i=0; i<ary.length; i++){
					arr=ary[i].split(':');
					key=parseInt(isNaN(arr[0]) ? (i==0?arr[0].substr(1,arr[0].length-2):arr[0].substr(0,arr[0].length-1)): arr[0]);//确保键是数字
					if(arr[1].substr(0,1)=='['){//值为数组
						result[key]=arr[1].substr(1,arr[1].length-2).split(',');
					}else{
						result[key]=arr[1];
					}
				}
			}
		}else{
			result=value;
		}
		return result;
	},
	
	json_decode_data:function(value){
		var result;
		if(typeof(value)=='object'){
			if(typeof(JSON)=='object'){
				result=JSON.stringify(value);
			}else{
				result='';
				for(k in value){
					if(global_obj.is_array(value[k])){
						result+=('"'+k+'":['+value[k]+'],');
					}else{
						result+=('"'+k+'":"'+value[k]+'",');
					}
				}
				result='{'+result.substr(0,result.length-1)+'}';
			}
		}else{
			result=value;
		}
		return result;
	},
	
	in_array:function(needle, arr){
		for(var i=0; i<arr.length && arr[i]!=needle; i++); 
		return !(i==arr.length);
	},
	
	is_array:function(data){
		if(Object.prototype.toString.call(data)=='[object Array]'){
			return true;
		}else{
			return false;
		}
	},
	
	base64_encode:function(str){
		var c1, c2, c3;
		var base64EncodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";               
		var i = 0, len= str.length, string = '';
		while (i < len){
			c1 = str.charCodeAt(i++) & 0xff;
			if (i == len){
				string += base64EncodeChars.charAt(c1 >> 2);
				string += base64EncodeChars.charAt((c1 & 0x3) << 4);
				string += "==";
				break;
			}
			c2 = str.charCodeAt(i++);
			if (i == len){
				string += base64EncodeChars.charAt(c1 >> 2);
				string += base64EncodeChars.charAt(((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4));
				string += base64EncodeChars.charAt((c2 & 0xF) << 2);
				string += "=";
				break;
			}
			c3 = str.charCodeAt(i++);
			string += base64EncodeChars.charAt(c1 >> 2);
			string += base64EncodeChars.charAt(((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4));
			string += base64EncodeChars.charAt(((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6));
			string += base64EncodeChars.charAt(c3 & 0x3F)
		}
		return string
	},
	
	htmlspecialchars:function(str){            
		str = str.replace(/&/g, '&amp;');
		str = str.replace(/</g, '&lt;');
		str = str.replace(/>/g, '&gt;');
		str = str.replace(/"/g, '&quot;');
		str = str.replace(/'/g, '&#039;');
		return str;
	},
	 
	htmlspecialchars_decode:function(str){           
		str = str.replace(/&amp;/g, '&');
		str = str.replace(/&lt;/g, '<');
		str = str.replace(/&gt;/g, '>');
		str = str.replace(/&quot;/g, '"');
		str = str.replace(/&#039;/g, "'");
		return str;
    },
	
	setCookie:function(name, value, expiredays){
		var exdate=new Date();
		exdate.setDate(exdate.getDate()+expiredays);
		alert(name+"="+escape(value)+((expiredays==null) ? "" : ";expires="+exdate.toGMTString()));
		document.cookie=name+"="+escape(value)+((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
	},
	
	getCookie:function(name){
		if(document.cookie.length>0){
			start=document.cookie.indexOf(name+"=");
			if(start!=-1){
				start=start+(name.length+1);
				end=document.cookie.indexOf(";", start);
				if(end==-1) end=document.cookie.length;
				return unescape(document.cookie.substring(start, end));
			}
		}
		return "";
	},
	
	delCookie:function(name, expiredays){
		var exdate=new Date();
		exdate.setDate(exdate.getDate()-1);
		var value=global_obj.getCookie(name)
		if(value){
			document.cookie=name+"="+escape(value)+((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
		}
	},
}

//Object => string
$.toJSON = typeof JSON == "object" && JSON.stringify ? JSON.stringify: function(e) {
	if (e === null) return "null";
	var t, n, r, i, s = $.type(e);
	if (s === "undefined") return undefined;
	if (s === "number" || s === "boolean") return String(e);
	if (s === "string") return $.quoteString(e);
	if (typeof e.toJSON == "function") return $.toJSON(e.toJSON());
	if (s === "date") {
		var o = e.getUTCMonth() + 1,
		u = e.getUTCDate(),
		a = e.getUTCFullYear(),
		f = e.getUTCHours(),
		l = e.getUTCMinutes(),
		c = e.getUTCSeconds(),
		h = e.getUTCMilliseconds();
		o < 10 && (o = "0" + o);
		u < 10 && (u = "0" + u);
		f < 10 && (f = "0" + f);
		l < 10 && (l = "0" + l);
		c < 10 && (c = "0" + c);
		h < 100 && (h = "0" + h);
		h < 10 && (h = "0" + h);
		return '"' + a + "-" + o + "-" + u + "T" + f + ":" + l + ":" + c + "." + h + 'Z"'
	}
	t = [];
	if ($.isArray(e)) {
		for (n = 0; n < e.length; n++) t.push($.toJSON(e[n]) || "null");
		return "[" + t.join(",") + "]"
	}
	if (typeof e == "object") {
		for (n in e) if (hasOwn.call(e, n)) {
			s = typeof n;
			if (s === "number") r = '"' + n + '"';
			else {
				if (s !== "string") continue;
				r = $.quoteString(n)
			}
			s = typeof e[n];
			if (s !== "function" && s !== "undefined") {
				i = $.toJSON(e[n]);
				t.push(r + ":" + i)
			}
		}
		return "{" + t.join(",") + "}"
	}
};

//string => Object
$.evalJSON = typeof JSON == "object" && JSON.parse ? JSON.parse: function(str) {
	return eval("(" + str + ")")
};

var //escape=/["\\\x00-\x1f\x7f-\x9f]/g,
	meta={
		'\b': '\\b',
		'\t': '\\t',
		'\n': '\\n',
		'\f': '\\f',
		'\r': '\\r',
		'"': '\\"',
		'\\': '\\\\'
	},
	hasOwn=Object.prototype.hasOwnProperty;
	
$.quoteString = function (str) {
	if (str.match(escape)) {
		return '"' + str.replace(escape, function (a) {
			var c = meta[a];
			if (typeof c === 'string') {
				return c;
			}
			c = a.charCodeAt();
			return '\\u00' + Math.floor(c / 16).toString(16) + (c % 16).toString(16);
		}) + '"';
	}
	return '"' + str + '"';
};