/*
Powered by ly200.com		http://www.ly200.com
广州联雅网络科技有限公司		020-83226791
*/

$(document).ready(function(){
	$('#blog_form').submit(function(){return false;});
	$('#blog_form input:submit').click(function(){
		if(global_obj.check_form($('*[notnull]'))){return false;};
		$('#blog_form input:submit').attr('disabled', true);

		$.post('?', $('#blog_form').serializeArray(), function(data){
			if (data.ret!=1){
				global_obj.win_alert(data.msg);
			}else{
				global_obj.win_alert(data.msg, function(){
					window.location=window.location.href;
				});
			}
		}, 'json');
	});
});
