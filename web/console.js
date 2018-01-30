if (typeof console != "undefined")
    if (typeof console.log != 'undefined')
        console.olog = console.log;
    else
        console.olog = function () { };

let i = 1;
console.log = function (message) {
    console.olog(message);

    $('#debugDiv').append(i++ + ': ' + message + '<br>');
    $("#debugDiv").scrollTop($("#debugDiv")[0].scrollHeight);
};

console.error = console.debug = console.info = console.log