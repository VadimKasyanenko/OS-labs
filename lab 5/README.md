# Лабораторная работа №5
## Тема: «Обмен данными по анонимному каналу с сервером»
 
## Индивидуальное задание: 
Написать программы для консольных процессов Server и Alfavit, которые обмениваются сообщениями по
анонимным каналам (2 шт.): 1 процесс записывает в первый канал, читает из второго канала, 2 процесс записывает во
второй канал, читает из первого канала .Создать наследуемый дескриптор канала.
Одновременно сообщение может передаваться только одним из процессов.

## Процесс- Server, который выполняет следующие действия:
- Размер массива и элементы массива вводятся с консоли. Тип массива: char
- Число N вводится с консоли
- Запускает процесс Alfavit l.
- Передаёт размер массива и элементы массива процессу Alfavit.
- Получает массив от процесса Alfavit;
- Выводит переданную и полученную информацию по каналу на консоль.

## Процесс-Alfavit, который выполняет следующие действия.
- Получает массив символов по анонимному каналу от процесса-сервера.
- Определяет символы, принадлежащие латинскому алфавиту и передает их по анонимному каналу процессусерверу.
- Выводит оба массива на консоль.
- Элементы массива передаются поэлементно.