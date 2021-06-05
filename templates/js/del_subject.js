function del_subject(e) {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/GroupAPI/DeleteSubject', false);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function() {
        if(xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
            e.parentElement.remove();
        } else {
            alert(xhr.response);
        }
    }
    xhr.send(`id=${e.dataset.sid}`);
}