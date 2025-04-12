import { createClient } from 'webdav';
import fs from 'fs';
import path from 'path';

// Настройки для подключения к WebDAV
const url = 'https://webdav.yandex.ru'; // Используйте правильный URL для WebDAV
const username = 'vivsiii'; // Ваше имя пользователя из переменной окружения
const password = 'zoahopioekddjatl'; // Ваш пароль из переменной окружения

console.log(`Username: ${username}, Password: ${password}`); // Проверка учетных данных

const client = createClient(url, {
    username,
    password
});

// Функция для создания папки
async function createFolder(folderName) {
    await client.createDirectory(folderName);
    console.log(`Папка "${folderName}" создана.`);
}

// Функция для загрузки файла
async function uploadFile(localFilePath, remoteFilePath) {
    if (!fs.existsSync(localFilePath)) {
        console.error(`Файл "${localFilePath}" не найден.`);
        return;
    }
    await client.putFileContents(remoteFilePath, fs.readFileSync(localFilePath));
    console.log(`Файл "${localFilePath}" загружен как "${remoteFilePath}".`);
}

// Функция для загрузки файла
async function downloadFile(remoteFilePath, localFilePath) {
    const data = await client.getFileContents(remoteFilePath);
    fs.writeFileSync(localFilePath, data);
    console.log(`Файл "${remoteFilePath}" загружен в "${localFilePath}".`);
}

// Функция для копирования файла
async function copyFile(sourcePath, destinationPath) {
    await client.copyFile(sourcePath, destinationPath);
    console.log(`Файл "${sourcePath}" скопирован в "${destinationPath}".`);
}

// Функция для удаления файла
async function deleteFile(filePath) {
    await client.deleteFile(filePath);
    console.log(`Файл "${filePath}" удален.`);
}

// Функция для удаления папки
async function deleteFolder(folderPath) {
    // Удаление всех файлов в папке перед удалением самой папки
    const files = await client.getDirectoryContents(folderPath);
    for (const file of files) {
        await client.deleteFile(file.filename); // Удаление каждого файла
    }
    await client.deleteFile(folderPath); // Удаление самой папки
    console.log(`Папка "${folderPath}" удалена.`);
}

// Пример использования
(async () => {
    try {
        await createFolder('test-folder');
        await uploadFile(path.join(process.cwd(), 'local-file.txt'), 'test-folder/remote-file.txt');
        await downloadFile('test-folder/remote-file.txt', 'downloaded-file.txt');
        await copyFile('test-folder/remote-file.txt', 'test-folder/remote-file-copy.txt');
        await deleteFile('test-folder/remote-file-copy.txt');
        await deleteFolder('test-folder');
    } catch (error) {
        console.error(error);
    }
})();