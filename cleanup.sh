#!/bin/bash

# 腳本第一行，宣告這是一個 bash 腳本

# ----------------------------------------------------
# Zephyr 專案清理腳本 (Cleanup Script)
# ----------------------------------------------------

# 設定目標建置資料夾的名稱
BUILD_DIR="../zephyrproject/build_hello"

# 檢查建置資料夾是否存在
if [ ! -d "$BUILD_DIR" ]; then
  echo "建置資料夾 '$BUILD_DIR' 不存在，無需清理。"
  exit 0
fi

echo "正在清理 '$BUILD_DIR'..."
# 使用 'find' 指令，來刪除除了特定檔案之外的所有內容
# 這是最安全、最精準的作法
find "$BUILD_DIR" -type f \
  ! -name "zephyr.elf" \
  ! -name "zephyr.bin" \
  ! -name "zephyr.hex" \
  ! -name "zephyr.map" \
  -exec rm -f {} +

echo "清理完成！只保留了 zephyr.elf, .bin, .hex, .map 檔案。"
