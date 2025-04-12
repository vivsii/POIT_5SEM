const sendmail = require('sendmail')();
const nodemailer = require('nodemailer');
const transporter = nodemailer.createTransport({
    host: 'smtp.ethereal.email',
    port: 587,
    auth: {
        user: 'benedict63@ethereal.email',
        pass: 'P1yezcUyxVrdmfFSf7'
    }
});

const fixedEmail = 'ben63@ethereal.emailm';

function send(message) {
    transporter.sendMail({
        from: fixedEmail,
        to: fixedEmail,
        subject: 'Сообщение от Node.js',
        text: message,
    }, (err, info) => {
        if (err) {
            res.writeHead(500, { 'Content-Type': 'text/plain' });
            res.end('Error: ' + err.toString());
            return;
        }
        res.writeHead(200, { 'Content-Type': 'text/plain' });
        res.end('Email was sent: ' + info.response);
    });
}

module.exports = { send };
