   SELECT IIF(ISNULL(COUNT(*)), 0, COUNT(*)) FROM ARRANJO_02_04 WHERE intSuporteId = [p_SuporteId];
