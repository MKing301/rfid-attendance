var keywords = [
    "Automation",
    "Accountability",
    "Improved Processing",
    "Efficient Reporting"
];

var count = 1;

setInterval(function(){
    $("span.keyword").fadeOut(400,
        function(){
            $(this).html(keywords[count]);
            count++;
            if(count == keywords.length)
                count = 0;
            $(this).fadeIn(400);
        });
}, 2000);