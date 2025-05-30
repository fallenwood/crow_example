<script setup lang="ts">
import {ref} from 'vue';

const greeting = ref("Hello World");

async function fetchGreeting() {
  const response = await fetch("/greeting");
  const data = await response.text();
  greeting.value = data;
}
</script>

<template>
  <div class="value">
  {{ greeting }}
  </div>
  <button @click="fetchGreeting()">click me</button>
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}
.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}
.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}
</style>
