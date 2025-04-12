--create database EVP
CREATE TABLE Faculty (
    faculty_id INT IDENTITY(1,1) PRIMARY KEY,
    faculty_name NVARCHAR(100) NOT NULL
);

CREATE TABLE Pulpit (
    pulpit_id INT IDENTITY(1,1) PRIMARY KEY,
    pulpit_name NVARCHAR(100) NOT NULL,
    faculty_id INT FOREIGN KEY REFERENCES Faculty(faculty_id)
);

CREATE TABLE Auditorium_Type (
    auditorium_type_id INT IDENTITY(1,1) PRIMARY KEY,
    auditorium_type_name NVARCHAR(100) NOT NULL
);

CREATE TABLE Subject (
    subject_id INT IDENTITY(1,1) PRIMARY KEY,
    subject_name NVARCHAR(100) NOT NULL,
    pulpit_id INT FOREIGN KEY REFERENCES Pulpit(pulpit_id)
);

CREATE TABLE Auditorium (
    auditorium_id INT IDENTITY(1,1) PRIMARY KEY,
    auditorium_name NVARCHAR(100) NOT NULL,
    auditorium_capacity INT NOT NULL,
    auditorium_type_id INT FOREIGN KEY REFERENCES Auditorium_Type(auditorium_type_id)
);

CREATE TABLE Teacher (
    teacher_id INT IDENTITY(1,1) PRIMARY KEY,
    teacher_name NVARCHAR(100) NOT NULL
);

INSERT INTO Faculty (faculty_name) VALUES
('Факультет математики'),
('Факультет информатики');

INSERT INTO Pulpit (pulpit_name, faculty_id) VALUES 
('Кафедра информатики', 2), 
('Кафедра высшей математики', 1),
('Кафедра компьютерных наук', 2);

INSERT INTO Teacher (teacher_name) VALUES
('Иванов Иван Иванович'),
('Петрова Анна Сергеевна');

INSERT INTO Auditorium_Type (auditorium_type_name) VALUES
('Лекционный'),
('Практический');

INSERT INTO Auditorium (auditorium_name, auditorium_capacity, auditorium_type_id) VALUES
('Аудитория 101', 30, 1),
('Аудитория 102', 50, 2);

INSERT INTO Subject (subject_name, pulpit_id) VALUES
('Математический анализ', 1),
('Программирование', 2);

INSERT INTO Auditorium_Type (auditorium_type_name) VALUES
('Лекционный'),
('Практический');

SELECT * FROM Pulpit;
SELECT * FROM Auditorium;
SELECT * FROM Faculty;
SELECT * FROM Subject;
SELECT * FROM Teacher;
