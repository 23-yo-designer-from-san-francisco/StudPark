AWS.config.update({
  region: bucketRegion,
  credentials: new AWS.CognitoIdentityCredentials({
    IdentityPoolId: IdentityPoolId
  })
});

var s3 = new AWS.S3({
  params: {
    Bucket: albumBucketName
  }
});

function randstr() {
  const length = 10;
  var result = [];
  var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  var charactersLength = characters.length;
  for (var i = 0; i < length; i++) {
    result.push(characters.charAt(Math.floor(Math.random() *
      charactersLength)));
  }
  return result.join('');
}

function uploadFile() {
  var btnupload = document.getElementById('btn-upload');
  var fileupload = document.getElementById("fileupload");
  var files = fileupload.files;
  if (!files.length) {
    return alert("Сначала выберите файл.");
  }
  btnupload.disabled = true;
  var file = files[0];
  var fileName = file.name;
  var albumPhotosKey = encodeURIComponent(albumName) + "/";

  var photoKey = albumPhotosKey + fileName;

  let pos = photoKey.lastIndexOf('.');
  var name = `${photoKey.substr(0, pos-1)}_${randstr()}${photoKey.substr(pos)}`

  var upload = new AWS.S3.ManagedUpload({
    params: {
      Bucket: albumBucketName,
      Key: name,
      Body: file
    }
  });

  var promise = upload.promise();

  promise.then(
    function(data) {
      document.getElementById('body').value += `\nСсылка: <a href=${data['Location']}>${fileName}</a>`;
      fileupload.value = "";
      alert("Файл успешно загружен в облако");
      btnupload.disabled = false;
    },
    function(err) {
      return alert("Произошла ошибка при загрузке файла: ", err.message);
      btnupload.disabled = false;
    }
  );
}