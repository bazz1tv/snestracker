jQuery(function($) {
	var HEADER_NAV_OPACITY = 0.3;
	var HEADER_NAV_SCROLL_WAIT = 3000;
	var HEADER_NAV_MOUSEOVER_WAIT = 0;
	var HEADER_NAV_MOUSEOUT_WAIT = 1000;

	var _autoScrollToTopTID = -1;
	var _headerNavOffsetTop = $('header nav').offset().top;
	var _headerNavOffsetHeight = $('header nav').height();
	var _headerNavScrollTop = _headerNavOffsetTop + _headerNavOffsetHeight + 30;
	var _headerNavFadeTID = 0;
	var _headerNavOpacityTID = 0;

	function autoScrollToTop() {
		var ht = $('html').scrollTop(), bt = $('body').scrollTop();
		var top = ht || bt;
		if (top <= 0) {
			clearInterval(_autoScrollToTopTID);
			_autoScrollToTopTID = -1;
		} else {
			$(ht ? 'html' : 'body').scrollTop(Math.floor(top / 1.3));
		}
	}

	$('#top_link').click(function() {
		if (_autoScrollToTopTID < 0) _autoScrollToTopTID = setInterval(autoScrollToTop, 10);
		return false;
	});

	$('header nav').mouseover(function() {
		if (!_headerNavFadeTID) return;

		if (_headerNavOpacityTID) clearTimeout(_headerNavOpacityTID);
		_headerNavOpacityTID = setTimeout(function() {
			$('header nav').stop(true, true).animate({opacity: 1}, 200);
		}, HEADER_NAV_MOUSEOVER_WAIT);
	}).mouseout(function() {
		if (!_headerNavFadeTID) return;

		if (_headerNavOpacityTID) clearTimeout(_headerNavOpacityTID);
		_headerNavOpacityTID = setTimeout(function() {
			$('header nav').stop(true, true).animate({opacity: HEADER_NAV_OPACITY}, 200);
		}, HEADER_NAV_MOUSEOUT_WAIT);
	});

	$(window).resize(function() {
		var main = $('main:first').css('min-height', '');
		var wh = $(window).innerHeight(), hh = $('html').height(), dh = wh - hh;
		if (dh > 0) main.css('min-height', (main.height() + dh) + 'px');
	}).resize().scroll(function() {
		if (_headerNavOpacityTID) clearTimeout(_headerNavOpacityTID);
		if (_headerNavFadeTID) {
			clearTimeout(_headerNavFadeTID);
			$('header nav').stop(true, true).css('top', '0px').animate({opacity: 1}, 0);
			_headerNavFadeTID = 0;
		}

		var top = $(this).scrollTop();
		if (top <= _headerNavScrollTop) return;

		top -= _headerNavOffsetTop;
		_headerNavFadeTID = setTimeout(function() {
			$('header nav').stop(true, true).css('top', top + 'px').animate({opacity: 0}, 0).animate({opacity: HEADER_NAV_OPACITY}, 200);
		}, HEADER_NAV_SCROLL_WAIT);
	}).scroll();
});
