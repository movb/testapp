# Тестовое приложение
## Сборка:
```sh
⇒  mkdir build
⇒  cd build
⇒  cmake ..
⇒  make
```

## Запуск:
### Сервер
```sh
⇒  server/server
```
### Клиент
```sh
⇒  client/client
```

## Особенности

Индикация сервера устроена следующим образом: в случае если LED выключен, сервер ничего не отображает. Если включен, то отображает надпись вида **on/off red/green/blue 0/1/2/3/4/5**, в выбранном цвете и мигающей с выбранной частотой.

Сообщение об ошибке **Failed** сервер выдает в случае неверных параметров и также, когда приходит запрос на установку параметров, которые соответствуют уже выставленным (т.е. если установлен красный цвет индикатора, то если попытаться опять установить красный цвет, ответ будет **Failed**).