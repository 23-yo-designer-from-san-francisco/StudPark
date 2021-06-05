(function($) {
	"use strict";

$(document).ready(function(){
    get_subjects();
    get_deadlines();

    var date = new Date();
    var today = date.getDate();
    // Set click handlers for DOM elements
    $(".right-button").click({date: date}, next_year);
    $(".left-button").click({date: date}, prev_year);
    $(".month").click({date: date}, month_click);
    $("#add-button").click({date: date}, new_event);
    // Set current month as active
    $(".months-row").children().eq(date.getMonth()).addClass("active-month");
    init_calendar(date);
    var events = check_events(today, date.getMonth()+1, date.getFullYear());
    show_events(events, months[date.getMonth()], today);
    $(document).on('click', '.del-event', function(e) {
        const id = e.target.dataset.eid
        $.ajax({
            type: 'POST',
            data: {
                'id': id
            },
            url: '/GroupAPI/DeleteDeadline',
            success: (data) => {
                if (data != "") {
                    alert(data);
                } else {
                    event_data["events"] = event_data["events"].filter((e)=>{
                         return e.id != id
        		}
		    );
        	    init_calendar(date);
                }
            },
            async: false
        });
    })
});



// Initialize the calendar by appending the HTML dates
function init_calendar(date) {
    $(".tbody").empty();
    $(".events-container").empty();
    var calendar_days = $(".tbody");
    var month = date.getMonth();
    var year = date.getFullYear();
    var day_count = days_in_month(month, year);
    var row = $("<tr class='table-row'></tr>");
    var today = date.getDate();
    // Set date to 1 to find the first day of the month
    date.setDate(1);
    var first_day = date.getDay();
    // 35+firstDay is the number of date elements to be added to the dates table
    // 35 is from (7 days in a week) * (up to 5 rows of dates in a month)
    for(var i=0; i<35+first_day; i++) {
        // Since some of the elements will be blank, 
        // need to calculate actual date from index
        var day = i-first_day+1;
        // If it is a sunday, make a new row
        if(i%7===0) {
            calendar_days.append(row);
            row = $("<tr class='table-row'></tr>");
        }
        // if current index isn't a day in this month, make it blank
        if(i < first_day || day > day_count) {
            var curr_date = $("<td class='table-date nil'>"+"</td>");
            row.append(curr_date);
        }   
        else {
            var curr_date = $("<td class='table-date'>"+day+"</td>");
            var events = check_events(day, month+1, year);
            if(today===day && $(".active-date").length===0) {
                curr_date.addClass("active-date");
                show_events(events, months[month], day);
            }
            // If this date has any events, style it with .event-date
            if(events.length!==0) {
                curr_date.addClass("event-date");
            }
            // Set onClick handler for clicking a date
            curr_date.click({events: events, month: months[month], day:day}, date_click);
            row.append(curr_date);
        }
    }
    // Append the last row and set the current year
    calendar_days.append(row);
    $(".year").text(year);
}

// Get the number of days in a given month/year
function days_in_month(month, year) {
    var monthStart = new Date(year, month, 1);
    var monthEnd = new Date(year, month + 1, 1);
    return (monthEnd - monthStart) / (1000 * 60 * 60 * 24);    
}

// Event handler for when a date is clicked
function date_click(event) {
    $(".events-container").show(250);
    $("#dialog").hide(250);
    $(".active-date").removeClass("active-date");
    $(this).addClass("active-date");
    show_events(event.data.events, event.data.month, event.data.day);
};

// Event handler for when a month is clicked
function month_click(event) {
    $(".events-container").show(250);
    $("#dialog").hide(250);
    var date = event.data.date;
    $(".active-month").removeClass("active-month");
    $(this).addClass("active-month");
    var new_month = $(".month").index(this);
    date.setMonth(new_month);
    init_calendar(date);
}

// Event handler for when the year right-button is clicked
function next_year(event) {
    $("#dialog").hide(250);
    var date = event.data.date;
    var new_year = date.getFullYear()+1;
    $("year").html(new_year);
    date.setFullYear(new_year);
    init_calendar(date);
}

// Event handler for when the year left-button is clicked
function prev_year(event) {
    $("#dialog").hide(250);
    var date = event.data.date;
    var new_year = date.getFullYear()-1;
    $("year").html(new_year);
    date.setFullYear(new_year);
    init_calendar(date);
}

// Event handler for clicking the new event button
function new_event(event) {
    if($(".active-date").length===0)
        return;
    $("input").click(function(){
        $(this).removeClass("error-input");
    })
    $("#dialog input[type=text]").val('');
    $("#dialog input[type=number]").val('');
    $(".events-container").hide(250);
    $("#dialog").show(250);
    // Event handler for cancel button
    $("#cancel-button").click(function() {
        $("#name").removeClass("error-input");
        $("#count").removeClass("error-input");
        $("#dialog").hide(250);
        $(".events-container").show(250);
    });
    // Event handler for ok button
    $("#ok-button").unbind().click({date: event.data.date}, function() {
        var date = event.data.date;
        var name = $("#name").val().trim();
        var time = $("#time").val();
        var day = parseInt($(".active-date").html());
        var discipline = $("#discipline").val();
        
        // Basic form validation
        if(name.length === 0 || time.length === 0) {
            $("#name").addClass("error-input");
        }
        else {
            const time_regex = new RegExp('^([0-9]|1[0-9]|2[0-3]):[0-5][0-9]$');
            if (!time.match(time_regex)) {
                alert("Проверьте правильность ввода времени");
                return;
            }
            $("#dialog").hide(250);
    
            new_event_json(name, time, date, day, discipline);
            date.setDate(day);
            init_calendar(date);
        }
    });
}

// Adds a json event to event_data
function new_event_json(name, time, date, day, discipline) {
    var event = {
        "occasion": name,
        "time": time,
        "year": date.getFullYear(),
        "month": date.getMonth()+1,
        "day": day,
        "discipline": subjects.find(x => x.id == discipline)['name'],
    };
    let month_zero = date.getMonth() < 9 ? '0' : '';
    let day_zero = day < 10 ? '0' : '';

    $.ajax( {
	method: "POST",
	url: "/GroupAPI/AddDeadline",
	data: {
        name: name,
        time: `${date.getFullYear()}-${month_zero}${date.getMonth()+1}-${day_zero}${day} ${time}:00`,
        subject: discipline
	}
    }).done(function(e) {
	if (e != ""){
	    alert(e);
	    return;
	}
    });
	event_data["events"].push(event);
}

// Display all events of the selected date in card views
function show_events(events, month, day) {
    // Clear the dates container
    $(".events-container").empty();
    $(".events-container").show(250);
    
    // If there are no events for this date, notify the user
    if(events.length===0) {
        var event_card = $("<div class='event-card'></div>");
        var event_name = $(`<div class='event-name'>Ничего не запланировано на ${day} ${month}.</div>`);
        $(event_card).css({ "border-left": "10px solid #FF1744" });
        $(event_card).append(event_name);
        $(".events-container").append(event_card);
    }
    else {
        // Go through and add each event as a card to the events container
        for(var i=0; i<events.length; i++) {
            var event_card = $("<div class='event-card'></div>");
            var event_name = $("<div class='event-name'>"+events[i]["occasion"]+" по " + 
                events[i]["discipline"] 
                +" в </div>");
            var event_count = $(`<div class='event-count'>${events[i]["time"]}</div><span class="del-event" data-eid="${events[i]["id"]}" style="color: red;cursor: pointer">[X]</span>`);
            $(event_card).append(event_name).append(event_count);
            $(".events-container").append(event_card);
        }
    }
}

// Checks if a specific date has any events
function check_events(day, month, year) {
    var events = [];
    for(var i=0; i<event_data["events"].length; i++) {
        var event = event_data["events"][i];
        if(event["day"]===day &&
            event["month"]===month &&
            event["year"]===year) {
                events.push(event);
            }
    }
    return events;
}

function get_subjects() {
    
    $.ajax({
        type: 'GET',
        url: '/GroupAPI/GetSubjects',
        success: (data) => {
         var json = JSON.parse(data);
         json["subjects"].forEach(e=>{
            subjects.push(e);
            $("#discipline").append(`<option value="${e.id}" style="color: black;">${e.name}</option>`);
         });
        },
        async: false
    });
}

function get_deadlines() {

    $.ajax({
        type: 'GET',
        url: '/GroupAPI/GetDeadlines',
        success: (data) => {
     var json = JSON.parse(data);
     if (json["deadlines"]) {
     json["deadlines"].forEach(e=>{
        var date = new Date(e.date);
        
        //new_event_json(e.name, );
	let minutes = date.getUTCMinutes();
	let zero = minutes > 9 ? '' : '0';
        event_data["events"].push({
            "id": e.id,
            "occasion": e.name,
            "year": date.getFullYear(),
            "month": date.getUTCMonth() + 1,
            "day": date.getUTCDate(),
            "time": `${date.getHours()}:${zero}${minutes}`,
            "discipline": subjects.find(x => x.id == e.subject)['name'],
        });
     });
 }
    },
        async: false
    });

    $.get( "/GroupAPI/GetDeadlines",  );
}

// Given data for events in JSON format
var event_data = {
    "events": [
        // {
        //     "occasion": "Repeated Test Event",
        //     "year": 2021,
        //     "month": 5,
        //     "day": 10,
        //     "discipline"
        // },
    ]
};

var subjects = [];
var deadlines = [];

// const months = [ 
//     "January", 
//     "February", 
//     "March", 
//     "April", 
//     "May", 
//     "June", 
//     "July", 
//     "August", 
//     "September", 
//     "October", 
//     "November", 
//     "December" 
// ];

const months = [ 
    "Января", 
    "Февраля", 
    "Марта", 
    "Апреля", 
    "Мая", 
    "Июня", 
    "Июля", 
    "Августа", 
    "Сентября", 
    "Октября", 
    "Ноября", 
    "Декабря" 
];

})(jQuery);
