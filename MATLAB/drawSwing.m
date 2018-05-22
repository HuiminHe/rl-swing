function drawSwing(t, z, p, u)
clf; hold on;
length = p.l1 + p.l2 + p.d2;
axis equal; axis(length* [-1, 1, -1, 1]);

[pc1, pc2, p1, p2, p3] = swingKinematics(z, p);

% pivot
plot(0, 0,'ks','MarkerSize',25,'LineWidth',4);

% links
plot([0, p1(1)], [0, p1(2)], 'Color',[0.1, 0.8, 0.1],'LineWidth',4);
plot([p1(1), p3(1)], [p1(2), p3(2)], 'Color',[0.1, 0.8, 0.1],'LineWidth',4);
plot([p3(1), p2(1)], [p3(2), p2(2)], 'Color',[0.1, 0.8, 0.1],'LineWidth',4);

% joints
plot(0, 0, 'b', 'MarkerSize', 10);
plot(p1(1), p1(2), 'r', 'MarkerSize', 20);
plot(p2(1), p2(2), 'r', 'MarkerSize', 20);
% mass centers
plot(pc1(1), pc1(2),'k.','MarkerSize',20);
plot(pc2(1), pc2(2),'k.','MarkerSize',20);

title(sprintf('Swing, t = %6.4f u=%i',t, u));

drawnow;

end