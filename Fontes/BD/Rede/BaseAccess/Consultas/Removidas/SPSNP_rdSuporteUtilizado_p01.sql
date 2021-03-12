   SELECT IIF(ISNULL(COUNT(*)), 0, COUNT(*)) FROM ARRANJO_00_01 WHERE intSuporteId = [p_SuporteId];

