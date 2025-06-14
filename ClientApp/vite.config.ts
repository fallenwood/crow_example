import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vite.dev/config/
export default defineConfig({
  plugins: [vue()],
  server: {
    host: "0.0.0.0",
    proxy: {
      '/greeting': 'http://localhost:18080',
      '/api': 'http://localhost:18080',
    }
  }
});
