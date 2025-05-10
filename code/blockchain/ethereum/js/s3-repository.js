const { S3Client, PutObjectCommand } = require("@aws-sdk/client-s3");
const { format } = require("date-fns");

const s3 = new S3Client({ region: "eu-west-2" }); 


async function putObject(bucketName , key , dataObject) {

    const uploadParams = {
        Bucket: bucketName,
        Key: key,
        Body: JSON.stringify(dataObject),
        ContentType: "application/json"
    };

    try {
        const result = await s3.send(new PutObjectCommand(uploadParams));
        console.log("✅ PutObject", result);
    } catch (err) {
        console.error("❌ Error al subir:", err);
    }
}



async function addObject(bucketName , folder , dataObject) {

    const now = format(new Date(), "yyyyMMdd-HHmmss");
    const uploadParams = {
        Bucket: bucketName,
        Key: `${folder}/obj-${now}.json`,
        Body: JSON.stringify(dataObject),
        ContentType: "application/json"
    };

    try {
        const result = await s3.send(new PutObjectCommand(uploadParams));
        console.log("✅ PutObject", result);
    } catch (err) {
        console.error("❌ Error al subir:", err);
    }
}





module.exports = {
    S3Repository: { putObject , addObject }
};