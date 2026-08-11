# دليل إدارة الملفات في CMake باستخدام `target_sources`

يقدم هذا الدليل شرحاً لاستخدام طريقة `target_sources` في CMake لبناء وتنسيق المشروع بأسلوب حديث ومنظم.

---

## 1. ما هي طريقة `target_sources`؟

في CMake الحديث (Modern CMake)، بدلاً من وضع كل أسماء الملفات دفعة واحدة داخل `add_executable` أو استخدام `file(GLOB)`، يتم إنشاء الهدف (Target) أولاً ثم إضافة ملفات كل وحدة (Module) بشكل منظم ومستقل باستخدام `target_sources`.

---

## 2. الهيكل الحالي في `CMakeLists.txt`

```cmake
# 1. إنشاء الهدف الأساسي مع ملف main.cpp فقط
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src "${SQLITE3_DIR}")

# 2. إضافة ملفات السورس والهيدر لكل وحدة بشكل منفصل

# --- Module: Database ---
target_sources(${PROJECT_NAME} PRIVATE
    src/database/database.cpp
    src/database/database.h
)
```

---

## 3. ماذا تفعل عند إضافة ملف أو مجلد جديد؟

### أ) عند إضافة ملف جديد إلى وحدة موجودة (مثلاً إضافة `query.cpp` لمجلد `database`):
افتح [CMakeLists.txt](file:///c:/projects/wms/CMakeLists.txt) وأضف الملف تحت قسم الوحدة `target_sources`:

```cmake
# --- Module: Database ---
target_sources(${PROJECT_NAME} PRIVATE
    src/database/database.cpp
    src/database/database.h
    src/database/query.cpp    # <-- الملف الجديد
    src/database/query.h      # <-- الملف الجديد
)
```

### ب) عند إنشاء مجلد/وحدة جديدة بالكامل (مثلاً `src/inventory/`):
أنشئ ملفاتك داخل المجلد الجديد (`inventory.cpp` و `inventory.h`)، ثم أضف كتلـة `target_sources` جديدة خاصة بالوحدة في [CMakeLists.txt](file:///c:/projects/wms/CMakeLists.txt):

```cmake
# --- Module: Inventory ---
target_sources(${PROJECT_NAME} PRIVATE
    src/inventory/inventory.cpp
    src/inventory/inventory.h
)
```

---

## 4. مميزات طريقة `target_sources`

1. **الوضوح والنظام:** تنظيم الملفات في أقسام (Modules) واضحة ومقروءة تماماً.
2. **التحكم الدقيق:** تضمن عدم تجميع أي ملفات عشوائية أو مؤقتة عن طريق الخطأ.
3. **التشخيص السريع:** في حال وجود خطأ بناء، من السهل جداً معرفة أين يقع الملف والهدف التابع له.
4. **التوافق التام مع المعايير الحديثة (Modern CMake Standard).**
