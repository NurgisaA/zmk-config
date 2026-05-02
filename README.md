# ZMK Config — Lily58

Конфигурация прошивки [ZMK](https://zmk.dev/) для сплит-клавиатуры **Lily58** на контроллерах **nice!nano v2**.

## ⌨️ Раскладка

![Keymap](build/my_keymap.svg)

> **⇧BCLR** — Nav + Shift + ESC = BT Clear (без Shift = ESC)
> **⇧BNXT** — Nav + Shift + TAB = BT Next (без Shift = TAB)
>
> ⚠️ SVG может быть неактуальным — перегенерируй через [keymap-drawer](https://github.com/caksoylar/keymap-drawer) после изменений в keymap.

## ⚡ Особенности

- **Vim-навигация** — стрелки на HJKL (Nav layer)
- **Безопасный Bluetooth** — BT Clear и BT Next требуют удержания Shift (mod-morph)
- **Мультимедиа** — громкость, яркость, управление плеером на Nav layer
- **Оптимизация для Go** — `:=`, `()`, `{}`, `<>` удобно расположены на Code layer
- **Ctrl+Z/X/C/V** — undo/cut/copy/paste одной рукой на Code layer
- **ZMK Studio** — можно менять раскладку без перепрошивки (3 reserved слоя)
- **OLED дисплей** — показывает слой, батарею, WPM

## 🔧 Конфигурация

| Параметр | Значение |
|---|---|
| Контроллер | nice!nano v2 |
| Дисплей | OLED (battery, layer, WPM) |
| BT мощность | +8 dBm (максимум) |
| Debounce | 1ms press / 10ms release |
| ZMK Studio | включён, без блокировки |

## 🏗️ Сборка

Прошивка собирается автоматически через GitHub Actions при каждом push.

1. Сделать push в репозиторий
2. Перейти в **Actions** → последний workflow run
3. Скачать артефакт `firmware`
4. Прошить каждую половинку:
   - Подключить USB
   - Дважды нажать reset на nice!nano
   - Скопировать `.uf2` файл на появившийся диск

## 📁 Структура

```
zmk-config/
├── .github/workflows/build.yml  ← CI сборка прошивки
├── config/
│   ├── lily58.keymap            ← раскладка клавиш
│   ├── lily58.conf              ← конфигурация фич
│   └── west.yml                 ← манифест ZMK/Zephyr
├── boards/shields/              ← кастомные шилды (пусто)
├── build.yaml                   ← матрица сборки
└── build/                       ← локальные артефакты
```
