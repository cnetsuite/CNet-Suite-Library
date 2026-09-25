(function () {
  'use strict';

  function loadTime() {
    fetch('/api/time')
      .then(function (res) { return res.json(); })
      .then(function (data) {
        document.getElementById('server-time').textContent = data.time;
      })
      .catch(function () {
        document.getElementById('server-time').textContent = 'unavailable';
      });
  }

  function loadHits() {
    fetch('/api/hits')
      .then(function (res) { return res.json(); })
      .then(function (data) {
        document.getElementById('hit-count').textContent = data.hits;
      })
      .catch(function () {
        document.getElementById('hit-count').textContent = '?';
      });
  }

  function renderEntries(entries) {
    var list = document.getElementById('guestbook-entries');
    list.textContent = '';

    entries.forEach(function (entry) {
      var item = document.createElement('li');

      var name = document.createElement('div');
      name.className = 'entry-name';
      name.textContent = entry.name;

      var time = document.createElement('div');
      time.className = 'entry-time';
      time.textContent = entry.createdAt;

      var message = document.createElement('div');
      message.textContent = entry.message;

      item.appendChild(name);
      item.appendChild(time);
      item.appendChild(message);
      list.appendChild(item);
    });
  }

  function loadGuestbook() {
    fetch('/api/guestbook')
      .then(function (res) { return res.json(); })
      .then(function (data) { renderEntries(data.entries || []); })
      .catch(function () { /* leave the list as-is */ });
  }

  function showError(message) {
    var errorEl = document.getElementById('guestbook-error');
    errorEl.textContent = message;
    errorEl.hidden = false;
  }

  function clearError() {
    document.getElementById('guestbook-error').hidden = true;
  }

  function submitEntry(event) {
    event.preventDefault();
    clearError();

    var name = document.getElementById('guestbook-name').value.trim();
    var message = document.getElementById('guestbook-message').value.trim();

    if (!name || !message) {
      showError('Please fill in both your name and a message.');
      return;
    }

    fetch('/api/guestbook', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ name: name, message: message })
    })
      .then(function (res) {
        if (!res.ok) {
          throw new Error('request failed');
        }
        document.getElementById('guestbook-name').value = '';
        document.getElementById('guestbook-message').value = '';
        loadGuestbook();
      })
      .catch(function () {
        showError('Could not save your entry. Please try again.');
      });
  }

  document.addEventListener('DOMContentLoaded', function () {
    loadTime();
    loadHits();
    loadGuestbook();
    document.getElementById('guestbook-form').addEventListener('submit', submitEntry);
  });
})();
