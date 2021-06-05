function save(text) {
    if (text[0] != "№") {
        alert(text);
        return;
    }
    fileType = "text/csv";
    fileName = `${document.getElementById('title').innerHTML}.csv`;

  var blob = new Blob([text], { type: fileType });

  var a = document.createElement('a');
  a.download = fileName;
  a.href = URL.createObjectURL(blob);
  a.dataset.downloadurl = [fileType, a.download, a.href].join(':');
  a.style.display = "none";
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  setTimeout(function() { URL.revokeObjectURL(a.href); }, 1500);
}

function download() {
    const URL = '/GroupAPI/ExportList';
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            save(xmlHttp.responseText);
    }
    xmlHttp.open("GET", URL, true); // true for asynchronous 
    xmlHttp.send(null);
}