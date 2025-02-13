// Big Loop
function BigLoop1()
{
var txtArea = document.getElementById("textarea1");
txtArea.value = "";
for (var I = 0; I < 10000; I++)
txtArea.value += Text;
}

// Global Variables
var IdsetTimeout1 = null;
var IdInterval1 = null;
var Text = "a";

// setTimeout
function ExecsetTimeout1()
{
IdsetTimeout1 = setTimeout(ExecsetTimeout3, 100);
}

function ExecsetTimeout3()
{
var txtArea = document.getElementById("textarea1");
txtArea.value += Text;
//BigLoop1();
ExecsetTimeout1();
}

// setInterval
function ExecsetInterval1()
{
clearInterval(IdInterval1);
IdInterval1 = setInterval(ExecsetInterval3, 100);
}

function ExecsetInterval3()
{
var txtArea = document.getElementById("textarea2");
txtArea.value += Text;
//document.form1.textarea1.value += Text;
}

// Clear the timers.
function Stop()
{
clearTimeout(IdsetTimeout1);
clearInterval(IdInterval1);
}