# README - Archery Defender

## Giới thiệu

Archery Defender được phát triển bằng SDL2 và C++. Người chơi điều khiển một nhân vật ở giữa phía dưới màn hình, bắn mũi tên lên trên để tiêu diệt kẻ địch và boss. Trò chơi có menu, hệ thống điểm số, máu (HP), và các tính năng tương tác thú vị.

## Tính năng chính

- **Nhân vật**:
  - Đứng ở giữa phía dưới màn hình.
  - Di chuyển trái/phải bằng phím A/D.
  - Bắn mũi tên tự động mỗi 0,5 giây.
- **Kẻ địch**:
  - Xuất hiện ngẫu nhiên từ phía trên màn hình mỗi 2 giây.
  - Di chuyển xuống dưới và bắn laser khi ở gần nhân vật.
  - Có animation idle và tấn công.
- **Boss**:
  - Xuất hiện sau mỗi 40 giây, ở giữa phía trên màn hình.
  - Bắn vũ khí (`boss_hit.png`) mỗi 5 giây.
  - Cần 15 mũi tên để hạ gục, sau đó hiển thị animation chết.
- **Menu**:
  - Menu chính với "Start Game" và "Quit".
  - Menu "Game Over" với "Play Again" và "Quit" khi HP = 0.
- **Hệ thống điểm số và HP**:
  - Điểm số hiển thị ở góc trên bên trái.
  - HP hiển thị ở góc trên bên phải.
  - Nhân vật mất HP khi bị trúng laser hoặc vũ khí của boss.
- **Âm thanh**:
  - Âm thanh khi bắn trúng kẻ địch hoặc boss.

## Yêu cầu hệ thống

- **Hệ điều hành**: Windows
- **Thư viện**: SDL2, SDL2_image, SDL2_mixer, SDL2_ttf.
- **Trình biên dịch**: gcc hoặc g++.
- **Công cụ lập trình**: Code::Blocks.
## Cài đặt

1. **Cài đặt SDL2 và các thư viện liên quan**:

   - Bạn có thể tham khảo trên mạng cách cài đặt.

2. **Clone hoặc tải mã nguồn**:

   - Sao chép mã nguồn vào thư mục dự án.

3. **Biên dịch và chạy trò chơi**:

   - Bấm F9 để biên dịch và chạy trò chơi.

## Cách chơi

- **Menu**:
  - Dùng phím mũi tên lên/xuống để chọn "Start Game" hoặc "Quit".
  - Nhấn Enter để xác nhận.
- **Trong trò chơi**:
  - Dùng phím A/D để di chuyển nhân vật trái/phải.
  - Nhân vật tự động bắn mũi tên lên trên.
  - Tránh laser từ kẻ địch và vũ khí từ boss.
  - Tiêu diệt kẻ địch và boss để ghi điểm.
- **Game Over**:
  - Khi HP = 0, menu "Game Over" hiển thị.
  - Chọn "Play Again" để chơi lại hoặc "Quit" để thoát.

## Các file hình ảnh và âm thanh

- **Hình ảnh**:
  - `character_sprites.png`: Sprite của nhân vật.
  - `arrow.png`: Sprite của mũi tên.
  - `enemy_sprites.png`: Sprite idle của kẻ địch.
  - `enemy_attack_sprites.png`: Sprite tấn công của kẻ địch.
  - `laser.png`: Sprite của laser.
  - `boss_attack_sprites.png`: Sprite tấn công của boss.
  - `boss_death_sprites.png`: Sprite chết của boss.
  - `boss_hit.png`: Sprite vũ khí của boss.
- **Âm thanh**:
  - `hit.wav`: Âm thanh khi bắn trúng kẻ địch hoặc boss.
  - `boss_attack.wav`: Âm thanh khi boss tấn công.
  - `game_over.wav`: Âm thanh khi trò chơi kết thúc.
  - `game_start.wav`: Âm thanh khi trò chơi bắt đầu.
- **Font**:
  - `arial.ttf:` Font cho văn bản trong game.

## Ghi chú

- Đảm bảo tất cả các file hình ảnh và âm thanh nằm trong thư mục làm việc.
- Trò chơi có thể mở rộng thêm các tính năng như, hiệu ứng đặc biệt, hoặc hỗ trợ chuột.

## Tác giả

- Trò chơi được phát triển bởi Lê Ngọc Quang(K69-UET-VNU).
